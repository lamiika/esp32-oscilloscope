#define USER_SETUP_ID 7789

// ----------------------------------------------------
// Display Driver
// ----------------------------------------------------
#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Newhaven ST7789 specifics
#define TFT_RGB_ORDER TFT_BGR
#define TFT_INVERSION_ON
#define TFT_OFFSET_X 0
#define TFT_OFFSET_Y 0
#define LOAD_GLCD

// ----------------------------------------------------
// Pin Mapping (LilyGO T-Display external)
// ----------------------------------------------------
#define TFT_MOSI  13
#define TFT_SCLK  12
#define TFT_CS    15
#define TFT_DC    27

// ----------------------------------------------------
// SPI Settings
// ----------------------------------------------------
#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000
