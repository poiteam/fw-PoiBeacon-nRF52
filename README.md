# PoiBeacon

- Developed with nRF SDK 17.1.0
- Softdevice s132 Developed for nRF52832
- Softdevice s112 Developed for nRF52810

## Beacon Default Description

- Device Name                    = "nRF52832"
- Beacon Info Data Length        =  0x17   //                          1 Byte
- Device Type                    =  0x02   //  iBeacon                 1  Byte
- Advertising Data Length        =  0x15   //                          1  Byte       
- Beacon UUID                    =  8AA100000A46115FD94E5A966A3DDBB7   16 Byte
- Major                          =  1                                  2  Byte
- Minor                          =  0                                  2  Byte
- RSSI                           =  0xBD   // -67                      1  Byte   
- Advertising Interval           =  0x01C2 // 450                      2  Byte
- Tx Power                       =  -4                                 1  Byte

## Scan Response Description

telemetry_default_service_info[8]  = {0, 0, 0, 0, 0, 0, 0, 0};

telemetry_default_service_info[0]  = FIRMWARE_VERSION
telemetry_default_service_info[1]  = BATTERY_PERCENTAGE
telemetry_default_service_info[6]  = (ADVERTISING_INTERVAL && 0xFF00) >> 8
telemetry_default_service_info[7]  = (ADVERTISING_INTERVAL && 0x00FF)

### GATT Characteristics

OUR_SERVICE_UUID            0x1000 
PASS_CHARACTERISTC_UUID     0x1001 
UUID_CHARACTERISTC_UUID     0x1004 
BEACON_CHARACTERISTC_UUID   0x1005 


- i. Password Characteristic: First of all user must enter correct password to change  information about beacon this characteristic is handling the password control with control functions. Characteristic UUID = 0x1001. (5 bytes)
beacon_password  =  {0x41, 0x0D, 0x84, 0xFD, 0x4E}


- ii. Base-UUID Characteristic: To change the Base UUID information in the beacon, user need to send Base UUID values to the beacon via this characteristic and values needs to be 16 bytes hex values. Characteristic UUID = 0x1004

- iii. Beacon info Characteristic: User can change major-minor 4 bytes, advertising interval 2 bytes, Tx power 1 byte and APP RSSI (on 1 meter) 1 byte. All these informations are should written at once. Characteristic UUID = 0x1005.

#### Encryption Settings

- You can use MINOR_ENCRYPTION_STATUS variable in poi_sdk_config.h
  MINOR_ENCRYPTION_STATUS  0  // Encrypiton Disable
  MINOR_ENCRYPTION_STATUS  1  // Encryption Enable
