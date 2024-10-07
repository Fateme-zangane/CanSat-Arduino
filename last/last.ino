 

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <QMC5883LCompass.h>
QMC5883LCompass compass;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

Adafruit_MPU6050 mpu;
#include <SPI.h>
#include <SD.h>

File myFile;

// change this to match your SD shield or module;
const int chipSelect = 10;

void setup(void) {
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
   myFile = SD.open("test.txt", FILE_WRITE);
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //Serial.println("");
  delay(100);

  Serial.println("Pressure Sensor Test"); Serial.println("");  
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  // Initialize I2C.
  Wire.begin();
  // Initialize the Compass.
  compass.init();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Temperature:");
  Serial.print(temp.temperature);
  Serial.print("\tx-acceleration:");
  Serial.print(a.acceleration.x);
  Serial.print("\ty-acceleration:");
  Serial.print(a.acceleration.y);
  Serial.print("\tz-acceleration:");
  Serial.print(a.acceleration.z);
  Serial.print("\tx-gyro:");
  Serial.print(g.gyro.x);
  Serial.print("\ty-gyro:");
  Serial.print(g.gyro.y);
  Serial.print("\tz-gyro:");
  Serial.println(g.gyro.z);

  myFile.print("Temperature:");
  myFile.print(temp.temperature);
  myFile.print("\tx-acceleration:");
  myFile.print(a.acceleration.x);
  myFile.print("\ty-acceleration:");
  myFile.print(a.acceleration.y);
  myFile.print("\tz-acceleration:");
  myFile.print(a.acceleration.z);
  myFile.print("\tx-gyro:");
  myFile.print(g.gyro.x);
  myFile.print("\ty-gyro:");
  myFile.print(g.gyro.y);
  myFile.print("\tz-gyro:");
  myFile.println(g.gyro.z);

  
  

  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print(" Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    myFile.print(" Pressure:    ");
    myFile.print(event.pressure);
    myFile.println(" hPa");
   

    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print(" Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    myFile.print(" Temperature: ");
    myFile.print(temperature);
    myFile.println(" C");

    /* Convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print(" Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    Serial.println(" m");
    Serial.println("");
    myFile.print(" Altitude:    "); 
    myFile.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    myFile.println(" m");
    myFile.println("");
    
  }
  else
  {
    Serial.println("Sensor error");
  }
  
  int x, y, z,i;

  // Read compass values
  compass.read();

  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();
  i = compass.getAzimuth();

  Serial.print("X: ");
  Serial.print(x);
  Serial.print("   Y: ");
  Serial.print(y);
  Serial.print("   Z: ");
  Serial.print(z);
  Serial.print("   i: ");
  Serial.println(i);

  myFile.print("X: ");
  myFile.print(x);
  myFile.print("   Y: ");
  myFile.print(y);
  myFile.print("   Z: ");
  myFile.print(z);
  myFile.print("   i: ");
  myFile.println(i);
  myFile.close();

  delay(1000);
  
}
