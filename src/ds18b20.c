#include "ds18b20.h"

void delay_us(int us)
{
	usleep(us);
}

void delay_ms(int ms)
{
	usleep(ms * 1000);
}

void delay_s(int sec)
{
	sleep(sec);
}


long gpio_out(int pin, int val)
{
	char path[35];
	snprintf(path,35,"/sys/class/gpio/gpio%d/value", pin);
	int fd = open(path, O_WRONLY);
	char *hi = "1";
	char *lo = "0";
	int result;
	if(val)result = write(fd,hi,1);
	else result = write(fd,lo,1);
	return result;
	close(fd);
}

int gpio_read(int pin)
{
	char path[35];
	char val[3] = {'1','1','1'};
	snprintf(path,35,"/sys/class/gpio/gpio%d/value", pin);
 	int fd = open(path, O_RDONLY);
	read(fd, val, 3);
	close(fd);
	return atoi(val);
}

long gpio_export(int pin)
{
	ssize_t bytes_written;
	char buffer[3];
	int fd = open("/sys/class/gpio/export", O_WRONLY);
	bytes_written = snprintf(buffer, 3, "%d", pin);
	int result = write(fd, buffer, bytes_written);
	close(fd);
	return result;
}

long gpio_direct(int pin, char* direct)
{
	char path[35];
	snprintf(path,35,"/sys/class/gpio/gpio%d/direction", pin);
	// open file description
	int fd = open(path, O_WRONLY);
	int result = write(fd,direct,3);
	close(fd);
	return result;
}

void start()
{
	printf("%ld\n",gpio_export(ds_pin));
	// set out
	printf("%ld\n",gpio_direct(ds_pin,"out"));
	printf("%ld\n",gpio_out(ds_pin, 0));
	delay_us(480);

	printf("%ld\n",gpio_direct(ds_pin,"in"));
	delay_us(80);
	if(!gpio_read(ds_pin))printf("initial success");
	else printf("initial fail");
}

void write_one_wire(uint8_t byte_data)
{
	uint8_t current_byte_data = byte_data;
	for(int byte_idx = 0; byte_idx < 8; ++byte_idx)
	{	
		if(current_byte_data & (1 << byte_idx)!=0)
		{
			// write 1
			gpio_direct(ds_pin,"out");
			gpio_out(ds_pin, 1);
			delay_us(15);
			gpio_direct(ds_pin,"in");
			delay_us(50);
		}
		else
		{
			// write 0
			gpio_direct(ds_pin,"out");
			gpio_out(ds_pin, 0);
			delay_us(50);
			gpio_direct(ds_pin,"in");
		}
	}
}

uint8_t read_one_wire()
{
	uint8_t byte_data = 0x00;
	for(int byte_idx = 0; byte_idx < 8; byte_idx++)
	{
		gpio_direct(ds_pin,"out");
		gpio_out(ds_pin, 0);
		delay_us(2);
		gpio_direct(ds_pin,"in");
		if(gpio_read(ds_pin))byte_data |= (1 << (7 - byte_idx));
		delay_us(60);
	}
	return byte_data;
}

int main()
{
	start();
	delay_ms(10);
	write_one_wire(0xCC);
	write_one_wire(0x44);
	delay_ms(800);
	start();
	delay_ms(10);
	write_one_wire(0xCC);
	write_one_wire(0xBE);
	printf("%x",read_one_wire());
	return 0;
}


