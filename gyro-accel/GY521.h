#include<Wire.h>


#ifndef GY521_h
#define GY521_h


#define SELF_TEST_X 0x0D	//R/W [7:5]XA_TEST[4-2] [4:0]XG_TEST[4-0]
#define SELF_TEST_y 0x0E	//R/W [7:5]YA_TEST[4-2] [4:0]YG_TEST[4-0]
#define SELF_TEST_Z 0x0F	//R/W [7:5]ZA_TEST[4-2] [4:0]ZG_TEST[4-0]
#define SELF_TEST_A	0x10	//R/W [7:6]RESERVED [5:4]XA_TEST[1-0] [3:2]YA_TEST[1-0] [1:0]ZA_TEST[1-0]

#define GYRO_CONFIG 0x1B	//R/W [7]- [6]- [5]- [4:3]FS_SEL [1:0] [2]- [1]- [0]-
#define ACCEL_CONFIG 0x1C	//R/W [7]XA_ST [6]YA_ST [5]ZA_ST [4]AFS_SEL[1:0]

#define GY521_ADDR 0x68		//R/W - - - - -[2]GYRO_RESET [1]ACCEL_RESET [0]TEMP_RESET

#define PWR_MGMT_1 0x6B 	//R/W [7]DEVICE_RESET [6]SLEEP [5]CYCLE [4]- [3]TEMP_DIS [2:0]CLKSEL
#define PWR_MGMT_2 0x6C		//R/W [7:6]LP_WAKE_CTRL[1:0] [5]STBY_XA [4]STBY_YA [3]STBY_ZA [2]STBY_XG [1]STBY_YG [0]STBY_ZG

#define ACCEL_XOUT_H 0x3B   //R ACCEL_YOUT[15:8]
#define ACCEL_XOUT_L 0x3C   //R ACCEL_XOUT[7:0]

#define ACCEL_YOUT_H 0x3D   //R ACCEL_ZOUT[15:8]
#define ACCEL_YOUT_L 0x3E   //R ACCEL_YOUT[7:0]

#define ACCEL_ZOUT_H 0x3F   //R ACCEL_ZOUT[15:8]
#define ACCEL_ZOUT_L 0x40   //R ACCEL_ZOUT[7:0]

#define TEMP_OUT_H 0x41		//R TEMP_OUT[15:8]
#define TEMP_OUT_L 0x42		//R TEMP_OUT[7:0]

#define GYRO_XOUT_H 0x43    //R GYRO_XOUT[15:8]
#define GYRO_XOUT_L 0x44    //R GYRO_XOUT[7:0]

#define GYRO_YOUT_H 0x45 	//R GYRO_YOUT[15:8]
#define GYRO_YOUT_L 0x46	//R GYRO_YOUT[7:0]

#define GYRO_ZOUT_H 0x47	//R GYRO_ZOUT[15:8]
#define GYRO_ZOUT_L 0x48	//R GYRO_ZOUT[7:0]

enum accel_value_t
{
  ACCEL_2G, 
  ACCEL_4G, 
  ACCEL_8G, 
  ACCEL_16G, 
};

enum gyro_value_t
{
  GYRO_250DG,
  GYRO_500DG,
  GYRO_1000DG,
  GYRO_2000DG, 
};

 
class GY521
{
		public:
   
			GY521(gyro_value_t gyro_value, accel_value_t accel_value); 
      
			void read_accel();
			void read_gyro();
			void read_temp();
			void read_data();
      void print_accel_data();
      void print_gyro_data();
      void print_temp_data();
			bool init();
			void read_data(int filter_size);
			void read_accel(int filter_size);
			double get_x_accel();
			double get_y_accel();
			double get_z_accel();
			double get_x_gyro();
			double get_y_gyro();
			double get_z_gyro();
			double get_temp();
			void read_gyro(int filter_size);
			void read_temp(int filter_size);
      bool check_freefall();
      bool check_tilt();
			
		private:  
		
			int gyro_value;
			int accel_value;
			double accel_lsb;
			double gyro_lsb;
			double x_accel, y_accel, z_accel;
			double x_gyro, y_gyro, z_gyro;
			double temp;

			void read_reg(int16_t *data, int begin_addr);
			double convert_data(int total, int filter_size, double lsb);
			void init_reg(int reg, int value);
			void check_filter(int &filter_size);
			void mpu_not_found_error();
			bool detect_mpu();
		
			
};
#endif
