#define SELF_TEST_X 0x0D	//R/W [7:5]XA_TEST[4-2] [4:0]XG_TEST[4-0]
#define SELF_TEST_y 0x0E	//R/W [7:5]YA_TEST[4-2] [4:0]YG_TEST[4-0]
#define SELF_TEST_Z 0x0F	//R/W [7:5]ZA_TEST[4-2] [4:0]ZG_TEST[4-0]
#define SELF_TEST_A	0x10	//R/W [7:6]RESERVED [5:4]XA_TEST[1-0] [3:2]YA_TEST[1-0] [1:0]ZA_TEST[1-0]

#define GYRO_CONFIG 0x1B	//R/W [7]- [6]- [5]- [4:3]FS_SEL [1:0] [2]- [1]- [0]-
#define ACCEL_CONFIG 0x1C	//R/W [7]XA_ST [6]YA_ST [5]ZA_ST [4]AFS_SEL[1:0]

#define GY521_ADDR 0x68

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
 
 
struct GY521
{
  int16_t x_accel, y_accel, z_accel;
  int16_t x_gyro, y_gyro, z_gyro;
  int16_t temp;
};