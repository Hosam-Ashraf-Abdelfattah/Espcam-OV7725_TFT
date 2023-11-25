#include <ArduCAM.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// Pin definitions for the ArduCAM module
#define CS_PIN    13
#define MOSI_PIN  15
#define MISO_PIN  12
#define SCK_PIN   14
#define RESET_PIN 4
#define PWDN_PIN  2

// Pin definitions for the TFT screen
#define TFT_CS    5
#define TFT_DC    16
#define TFT_RST   -1 // Check TFT_RST pin if needed

// Global ArduCAM object
#define OV7725_CAM
// Directly specify image dimensions for display
const int IMAGE_WIDTH = 320; // Replace with the actual width of the image
const int IMAGE_HEIGHT = 240; // Replace with the actual height of the image
ArduCAM myCAM(OV7725, CS_PIN);  

// Global TFT_eSPI object
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  // Initialize TFT screen
  tft.begin();
  tft.setRotation(3); // Adjust screen rotation if necessary

  // Initialize the ArduCAM module
  //myCAM.set_CS_PIN(CS_PIN);
  myCAM.InitCAM();

  // Set camera format and resolution
  myCAM.set_format(JPEG);
  myCAM.InitCAM();

  // Set camera resolution and other settings
  //myCAM.OV7725_set_JPEG_size(OV7725_320x240);
  // Set other camera settings as desired (e.g., brightness, contrast, etc.)

  // Wait for the camera to stabilize
  delay(1000);
}

void loop() {
  // Capture an image
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.start_capture();

  // Wait for capture to complete
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));

  // Get the image size
  uint32_t length = myCAM.read_fifo_length();
  if (length >= MAX_FIFO_SIZE) {
    Serial.println("FIFO overflow!");
    return;
  }

  // Read the image data from the FIFO buffer
  byte* imageBuffer = reinterpret_cast<byte*>(myCAM.read_fifo());
  if (imageBuffer == NULL) {
    Serial.println("Failed to read image from FIFO!");
    return;
  }

  // Display the image on the TFT screen

  tft.pushImage(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, imageBuffer);

  // Delay to control frame rate
  delay(16); // Adjust the delay as needed for desired frame rate
}
