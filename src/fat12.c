#include "mem.h"
#include "term.h"
#include "kernel.h"
#include "fat12.h"
#include "io.h"

//Mostly taken from brokenthorn, hence the differing code style

static uint8_t	_CurrentDrive = 0;
const int FLPY_SECTORS_PER_TRACK = 18;
static volatile uint8_t _FloppyDiskIRQ = 0;
const int DMA_BUFFER = 0x1000;

void init_fat12(){
	flpydsk_initialize_dma();

	int sector_num = 0;
	uint8_t* sector = flpydsk_read_sector(sector_num);

	mem_dump(&tty0, sector, 32);
}

inline void flpydsk_wait_irq(){
	//! wait for irq to fire
	while ( _FloppyDiskIRQ == 0)
			;
	_FloppyDiskIRQ = 0;
}

void flpydsk_check_int(uint32_t* st0, uint32_t* cyl){
	flpydsk_send_command (FDC_CMD_CHECK_INT);

	*st0 = flpydsk_read_data ();
	*cyl = flpydsk_read_data ();
}

void flpydsk_control_motor(bool b){
	//! sanity check: invalid drive
	if (_CurrentDrive > 3)
		return;

	uint32_t motor = 0;

	//! select the correct mask based on current drive
	switch (_CurrentDrive) {

		case 0:
			motor = FLPYDSK_DOR_MASK_DRIVE0_MOTOR;
			break;
		case 1:
			motor = FLPYDSK_DOR_MASK_DRIVE1_MOTOR;
			break;
		case 2:
			motor = FLPYDSK_DOR_MASK_DRIVE2_MOTOR;
			break;
		case 3:
			motor = FLPYDSK_DOR_MASK_DRIVE3_MOTOR;
			break;
	}

	//! turn on or off the motor of that drive
	if (b)
		flpydsk_write_dor (_CurrentDrive | motor | FLPYDSK_DOR_MASK_RESET | FLPYDSK_DOR_MASK_DMA);
	else
		flpydsk_write_dor (FLPYDSK_DOR_MASK_RESET);

	//! in all cases; wait a little bit for the motor to spin up/turn off
	sleep (20);
}

//! initialize DMA to use phys addr 1k-64k
void flpydsk_initialize_dma(){
	outb(0x0a,0x06);	//mask dma channel 2
	outb(0xd8,0xff);	//reset master flip-flop
	outb(0x04, 0);     //address=0x1000 
	outb(0x04, 0x10);
	outb(0xd8, 0xff);  //reset master flip-flop
	outb(0x05, 0xff);  //count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(0x05, 0x23);
	outb(0x80, 0);     //external page register = 0
	outb(0x0a, 0x02);  //unmask dma channel 2
}
 
//! prepare the DMA for read transfer
void flpydsk_dma_read(){
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x56); //single transfer, address increment, autoinit, read, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}
 
//! prepare the DMA for write transfer
void flpydsk_dma_write(){
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x5a); //single transfer, address increment, autoinit, write, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}

void flpydsk_write_dor(uint8_t val){
	//! write the digital output register
	outb(FLPYDSK_DOR, val);
}

uint8_t flpydsk_read_status(){
	//! just return main status register
	return inb(FLPYDSK_MSR);
}

void flpydsk_send_command(uint8_t cmd){
	//! wait until data register is ready. We send commands to the data register
	for (int i = 0; i < 500; i++ )
		if ( flpydsk_read_status() & FLPYDSK_MSR_MASK_DATAREG )
			return outb(FLPYDSK_FIFO, cmd);
}
 
uint8_t flpydsk_read_data(){
	//! same as above function but returns data register for reading
	for (int i = 0; i < 500; i++ )
		if ( flpydsk_read_status() & FLPYDSK_MSR_MASK_DATAREG )
			return inb(FLPYDSK_FIFO);
}

void flpydsk_write_ccr(uint8_t val){
	//! write the configuation control
	outb (FLPYDSK_CTRL, val);
}

void flpydsk_read_sector_imp(uint8_t head, uint8_t track, uint8_t sector){
	uint32_t st0, cyl;
 
	//! set the DMA for read transfer
	flpydsk_dma_read ();
 
	//! read in a sector
	flpydsk_send_command (
		FDC_CMD_READ_SECT | FDC_CMD_EXT_MULTITRACK |
		FDC_CMD_EXT_SKIP | FDC_CMD_EXT_DENSITY);
	flpydsk_send_command ( head << 2 | _CurrentDrive );
	flpydsk_send_command ( track);
	flpydsk_send_command ( head);
	flpydsk_send_command ( sector);
	flpydsk_send_command ( FLPYDSK_SECTOR_DTL_512 );
	flpydsk_send_command (
		( ( sector + 1 ) >= FLPY_SECTORS_PER_TRACK )
			? FLPY_SECTORS_PER_TRACK : sector + 1 );
	flpydsk_send_command ( FLPYDSK_GAP3_LENGTH_3_5 );
	flpydsk_send_command ( 0xff );
 
	//! wait for irq
	flpydsk_wait_irq ();
 
	//! read status info
	for (int j=0; j<7; j++)
		flpydsk_read_data ();
 
	//! let FDC know we handled interrupt
	flpydsk_check_int (&st0,&cyl);
}

void flpydsk_drive_data(uint32_t stepr, uint32_t loadt, uint32_t unloadt, bool dma ){
	uint32_t data = 0;
 
	flpydsk_send_command (FDC_CMD_SPECIFY);
 
	data = ( (stepr & 0xf) << 4) | (unloadt & 0xf);
	flpydsk_send_command (data);
 
	data = (loadt) << 1 | (dma==true) ? 1 : 0;
	flpydsk_send_command (data);
}

int flpydsk_calibrate(uint32_t drive){
	uint32_t st0, cyl;
 
	if (drive >= 4)
		return -2;
 
	//! turn on the motor
	flpydsk_control_motor (true);
 
	for (int i = 0; i < 10; i++) {
 
		//! send command
		flpydsk_send_command ( FDC_CMD_CALIBRATE );
		flpydsk_send_command ( drive );
		flpydsk_wait_irq ();
		flpydsk_check_int ( &st0, &cyl);
 
		//! did we fine cylinder 0? if so, we are done
		if (!cyl) {
 
			flpydsk_control_motor (false);
			return 0;
		}
	}
 
	flpydsk_control_motor (false);
	return -1;
}

int flpydsk_seek(uint32_t cyl, uint32_t head ){
	uint32_t st0, cyl0;
 
	if (_CurrentDrive >= 4)
		return -1;
 
	for (int i = 0; i < 10; i++ ) {
 
		//! send the command
		flpydsk_send_command (FDC_CMD_SEEK);
		flpydsk_send_command ( (head) << 2 | _CurrentDrive);
		flpydsk_send_command (cyl);
 
		//! wait for the results phase IRQ
		flpydsk_wait_irq ();
		flpydsk_check_int (&st0,&cyl0);
 
		//! found the cylinder?
		if ( cyl0 == cyl)
			return 0;
	}
 
	return -1;
}

void flpydsk_disable_controller(){
	flpydsk_write_dor (0);
}

void flpydsk_enable_controller(){
	flpydsk_write_dor ( FLPYDSK_DOR_MASK_RESET | FLPYDSK_DOR_MASK_DMA);
}

void flpydsk_reset(){
	uint32_t st0, cyl;
 
	//! reset the controller
	flpydsk_disable_controller ();
	flpydsk_enable_controller ();
	flpydsk_wait_irq ();
 
	//! send CHECK_INT/SENSE INTERRUPT command to all drives
	for (int i=0; i<4; i++)
		flpydsk_check_int (&st0,&cyl);
 
	//! transfer speed 500kb/s
	flpydsk_write_ccr (0);
 
	//! pass mechanical drive info. steprate=3ms, unload time=240ms, load time=16ms
	flpydsk_drive_data (3,16,240,true);
 
	//! calibrate the disk
	flpydsk_calibrate ( _CurrentDrive );
}

void flpydsk_lba_to_chs(int lba,int *head,int *track,int *sector){
   *head = ( lba % ( FLPY_SECTORS_PER_TRACK * 2 ) ) / ( FLPY_SECTORS_PER_TRACK );
   *track = lba / ( FLPY_SECTORS_PER_TRACK * 2 );
   *sector = lba % FLPY_SECTORS_PER_TRACK + 1;
}

uint8_t* flpydsk_read_sector(int sectorLBA){
	if (_CurrentDrive >= 4)
		return 0;
 
	//! convert LBA sector to CHS
	int head=0, track=0, sector=1;
	term_writestring(&tty0, "\nCalculating location...\n");
	flpydsk_lba_to_chs(sectorLBA, &head, &track, &sector);
 
	//! turn motor on and seek to track
	flpydsk_control_motor (true);
	term_writestring(&tty0, "Spinning up...\n");
	if (flpydsk_seek(track, head) != 0){
		kernel_panic("Seek failure\n");
		return 0;
	}
 
	//! read sector and turn motor off
	term_writestring(&tty0, "Reading Sector...\n");
	flpydsk_read_sector_imp (head, track, sector);
	flpydsk_control_motor (false);
 
	//! warning: this is a bit hackish
	return (uint8_t*) DMA_BUFFER;
}