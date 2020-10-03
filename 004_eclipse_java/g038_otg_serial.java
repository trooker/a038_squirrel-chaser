package com.extensions.g038_otg_serial; 

/*  Copyright 2020 Abbottanp All rights reserved
 * 
 * Copyright 2016-2019 MIT, All rights reserved
 * Copyright 2017-2019 Kodular, All rights reserved
 * Released under the Apache License, Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 * With grateful thanks to 
 * Pavitra USB-Arduino work leading to this F310 OTG
 * Projecter at in/out_byte https://create.arduino.cc/projecthub/Projecter/android-usb-arduino-f68767
 * John Zeller python F310 parser work.
 * gtkterm team for USB terminal and hex character display
 * Harry Brown for masking technique
 * Helpful gamepad example by  Pixdmo at https://www.youtube.com/watch?v=ixQLcLxJLRc
 * Follow the process development model for final aix extension creation  https://medium.com/@saitwalshreyash19/writing-your-first-app-inventor-2-extension-dc6d5d4ff824
 *    Great model for creating failsafe extensions.  Start with the Gradient.java model and
 *    add content that has been locally built.
 * Good extesnion toolkit: https://github.com/mit-cml/extension-template
 * Salutes to Hariharan Mathavan work: https://www.allaboutcircuits.com/projects/communicate-with-your-arduino-through-android
 * and Hans Felhr for his effort in supporting the UsbSerial Library https://github.com/felHR85/UsbSerial/
 * 
 * 200929_tr Readjust error message to 100_0 50_M
 * 200927_tr Tweak the error message to 500_500_50_Z
 * 200925_tr Minor typos correctins.  Corrected incoming messages reporting an error condition to 
 *           use the same syntax used by the Android/a038_iii_gamepad_otg_wifi.aia to parse:
 *           "otg_msg_err_Z".
 * 200902_tr Restored USBSerial jar version.  Verified by building aix.
 * 200901_tr Swapped physicaloid jar for usbserial jar in g038_997...java 
 *           Testing with 20200701 failed.  
 *           Trying Pavita extensions usage.  Works once the "implements Components" is removed.
 *           Note using physicaloid from 2014.  Verified 2020 version worked. as well.
 * 200827_tr Taking a second look at Context/Activity processing in order to fold the Android
 *           example with the A2I extension syntax.
 * 200826_tr No real progress in "touching" the OTG device.  Reworked the A2I test app.
 * 200825_tr Major rewrite to add blocks as stubs and then the mystery functionality mCallback and broadCaster
 *           Tweaked the ANT editor.  The build.xml and the editor were double building targets.
 * 200824_tr Succeeded building through Eclipse-ant-build.xml.  Started adding methods, events, properties
 * 200823_tr Started to rebuild the extension with basic get/set for BufferSize.  It builds but throws 
 * 200821_tr Started blending the Arduino_Gamepad_OTG project (f038_898_app_gamepad.apk 
 *           with Hariharan Mathavan work: https://www.allaboutcircuits.com/projects/communicate-with-your-arduino-through-android
 *           see: https://github.com/hariharan-m/Arduino-Android-USB
 * 200812_tr 1> 994 version testing off-line app2inventor resulted in errors when attemtping
 *           to Save APK to computer due to "Build failed: refused connection". 
 *           2> 994_c038_868 ==> first contact with f310_otg-serial threw error message from build.
 * 
 */


import java.util.Arrays;


// add
import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.util.Log;


import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

//add
import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.Map;



//original
import com.google.appinventor.components.annotations.DesignerComponent;
import com.google.appinventor.components.annotations.DesignerProperty;
import com.google.appinventor.components.annotations.PropertyCategory;
import com.google.appinventor.components.annotations.SimpleEvent;
import com.google.appinventor.components.annotations.SimpleFunction;
import com.google.appinventor.components.annotations.SimpleObject;
import com.google.appinventor.components.annotations.SimpleProperty;
import com.google.appinventor.components.annotations.UsesLibraries;
import com.google.appinventor.components.common.ComponentCategory;
import com.google.appinventor.components.common.PropertyTypeConstants;
import com.google.appinventor.components.runtime.AndroidNonvisibleComponent;
import com.google.appinventor.components.runtime.AndroidViewComponent;
import com.google.appinventor.components.runtime.Component;
import com.google.appinventor.components.runtime.ComponentContainer;
import com.google.appinventor.components.runtime.EventDispatcher;


import com.google.appinventor.components.runtime.util.ErrorMessages;

// my empty bucket
import com.extensions.g038_otg_serial.utility.USB_Handler;



@DesignerComponent(version = 1,
                    category = ComponentCategory.EXTENSION,
                    description = "OTG-Serial component for connecting Orion Joystick to  an Android",
                    nonVisible = true,
                    iconName = "images/extenstion.png",
                    androidMinSdk = 12)

@SimpleObject (external = true)
@UsesLibraries(libraries = "usbserial.jar")

public class  g038_otg_serial extends AndroidNonvisibleComponent implements Component
{
	//felhr  big question mark
	public final String ACTION_USB_PERMISSION = "com.extensions.g038_otg_serial.USB_PERMISSION";            //"com.hariharan.arduinousb.USB_PERMISSION";
	private static final String LOG_TAG = "g038_otg_serial Component";
	private Context context;
	private int baudRate = 9600;
	private int bytes = 48; //200728 from 256;
	private String incomingMsg = null;
	private boolean initializationFlag = false;
	private boolean openFlag = false;
	private USB_Handler usbhandler;
	//android  
    UsbManager usbManager;
    UsbDevice device;
//felhr  
    UsbSerialDevice serialPort;
    UsbDeviceConnection connection;
 //hariharan Defining a Callback which triggers whenever data is read.
/*  */ 
   UsbSerialInterface.UsbReadCallback mCallback = new 
   UsbSerialInterface.UsbReadCallback()
   { 
        @Override
        public void onReceivedData(byte[] arg0)
        {
        	incomingMsg = null;
            try
            {
            	incomingMsg = new String(arg0, "UTF-8");
            	incomingMsg.concat("/n");
                //parse data to update JS0, Speed, ButtonPress
                Log.d(LOG_TAG, "Incoming MSG Received");
            } catch (UnsupportedEncodingException e)
            { e.printStackTrace();}
        }
    }; 
 
/*  */   
 	

  /**
   * Creates a Object Context or OTG-Serial communication pathway 
   * between an Arduino Orion UNO and the Android Host.  The 
   * communication once established will be "ONLY" one way 
   * to the Android host.
   * 
   * @param 
   */
  
  
  public g038_otg_serial(ComponentContainer container) {
    super(container.$form());
    context = container.$context();
//    usbManager = (UsbManager) context.getSystemService(context.USB_SERVICE);//this.USB_SERVICE);
//    IntentFilter filter = new IntentFilter();
//   filter.addAction(ACTION_USB_PERMISSION);
//    filter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
//    filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
//may not like the broadcastReceiver	
//try build 200826 
//    context.registerReceiver(broadcastReceiver, filter);
    Log.d(LOG_TAG, "Created Object Context");
  }
  
  
  /**
   * The mystery worker bee 
   */

  	private final BroadcastReceiver broadcastReceiver = new BroadcastReceiver()
     { //Broadcast Receiver to automatically start and stop the Serial connection.
   	 // @Override
         public void onReceive(Context context, Intent intent)
         {
      	   //begin processing serialPort for call back
      	      if (intent.getAction().equals(ACTION_USB_PERMISSION))
                {
                    boolean granted = intent.getExtras().getBoolean(UsbManager.EXTRA_PERMISSION_GRANTED);
                    if (granted)
                     {
                    	ErrorOccured("Granted Permission");
                        connection = usbManager.openDevice(device);
                        serialPort = UsbSerialDevice.createUsbSerialDevice(device, connection);
                        if (serialPort != null)
                         {
                            initializationFlag = true;
                        	ErrorOccured("serialPort != null. It lives.");
                            if (serialPort.open())
                             { //Set Serial Connection Parameters.
                            	ErrorOccured("serialPort openned.  Now setting baud, etc.");
                                // not usedsetUiEnabled(true);
                                serialPort.setBaudRate(baudRate);
                                serialPort.setDataBits(UsbSerialInterface.DATA_BITS_8);
                                serialPort.setStopBits(UsbSerialInterface.STOP_BITS_1);
                                serialPort.setParity(UsbSerialInterface.PARITY_NONE);
                                serialPort.setFlowControl(UsbSerialInterface.FLOW_CONTROL_OFF);
                                serialPort.read(mCallback);
                                ErrorOccured("Callback active.  Process to check for device listing.");
                                Log.d(LOG_TAG, "Connection Confirmed");
                            } 
                            else {Log.d("SERIAL", "PORT NOT OPEN");
                                   ErrorOccured("Port not opened");
                                   }
                        } 
                        else {Log.d("SERIAL", "PORT IS NULL");
                        ErrorOccured("Port is null");
                        }
                    } 
                    else {Log.d("SERIAL", "PERM NOT GRANTED");
                    ErrorOccured("No Permission Granted");
                    }
                } 
                else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_ATTACHED)) 
                { ErrorOccured("Going to OpenSerial");
                OpenSerial();}  // begin reading via mCallback
               else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_DETACHED)) 
               {CloseSerial();}
         //end processing serialPort for call back
      	 ;
         } // end void onReceive
     };  // end of declaration of BroadcastReceiver
 

/**
 * Establishes the actual connection for the Serial Port Android in the role
 * of host.  It has not yet attempted to determine whether any devices is 
 * connected that belong to the fehr driver list of devics.     
 */
     private void setConnection()
     {
      IntentFilter filter = new IntentFilter();
      filter.addAction(ACTION_USB_PERMISSION);
      filter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
      filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
      context.registerReceiver(broadcastReceiver, filter);
      Log.d(LOG_TAG, "Initialized Receiver Ready");
      
     } 
     
     

 /**
  * Set the IsIniitalized to True upon beginning a OTG-USB-Serial connection. 
  *   DO Not change Baud or Byte size.
  */
@SimpleFunction(description = "Initializes serial connection.")
  public void InitializeSerial()
  {
    //default USB Serial initialization of serialPort
    usbManager = (UsbManager) context.getSystemService(context.USB_SERVICE);//this.USB_SERVICE);
   }
  


/**
 * Check for the VID and PID to insure the device attached to the Host Android is a member in good standing
 * 200904_tr   
 * VID: // Must be an hex representation of 16 bit number (0000-FFFF). Don't worry about uppercase or lowercase
 *  https://felhr85.net/2014/06/27/get-vendor-and-product-name-from-vid-and-pid-in-android/
 *  Upon checking list for VID and PID lower case was found to be used.  
 * 
 * @return True if found in the USBSerial family of devices.
 */
 public boolean isDeviceAok() {
       boolean isDeviceGood2Go = false;
       HashMap<String, UsbDevice> usbDevices = usbManager.getDeviceList();
       if (!usbDevices.isEmpty()) {
           //boolean keep = true;  //not need here
           for (Map.Entry<String, UsbDevice> entry : usbDevices.entrySet()) {
               device = entry.getValue();
               ErrorOccured("Device: " + device.toString());
               int deviceVID = device.getVendorId();
               ErrorOccured("Device VID: " + deviceVID );
              //200713_tr
               
               if ((deviceVID == 0x2341)       //Arduino Vendor ID|| (deviceVID == 1133)|| (deviceVID == 1133)
                   || (deviceVID == 0x1a86)    //200820 Qinheng PID 7523  CH340
                   || (deviceVID == 0x046d))   //logitech hexadecimal F310 in list for Direct mode only but does not work
               {                                                  //context was this prior
                   PendingIntent pi = PendingIntent.getBroadcast(context, 0, new Intent(ACTION_USB_PERMISSION), 0);
                   usbManager.requestPermission(device, pi);
                   isDeviceGood2Go = true;  //keep = false;
                   Log.d("SERIAL", "DEVICE_VID Found but...");
               } else {
                   connection = null;
                   device = null;
                   Log.d("SERIAL", "NO DEVICE_VID");
                   //isDeviceGood2Go false by default
               }
           }
       } return isDeviceGood2Go;
   }



/**
 * Sets the IsOpen to True if the user opens the USB serial communication.
 
 * Fires Opening USB Serial process for checking the device's the VID/PID
 * to verifiied that it is supported by the driver libary.  If so this 
 * function returns True.  It sets the IsOpen to True
 * If True  are supported
 * @return
 */

 
  @SimpleFunction(description = "Opens serial connection. Returns true when opened.")
  public void OpenSerial()
  {
	Log.d(LOG_TAG, "Opening connection");
 	//Set Connection and CallBack for automatic listening of serial
	setConnection();  
    Log.d(LOG_TAG, "Opening connection");
	//Check listing of VID in fehr stable of devices
    openFlag = isDeviceAok();
    ErrorOccured("Return from isDeviceAok()");
    if (openFlag == true)
    {Log.d(LOG_TAG, "USB Open for business");
    ErrorOccured("Deviceis Aok");} 
    else                                                //old USB can bot be opened
    {ErrorOccured("Error occurred in the Opening Process.  VID not found");}  
  }
  
  
  
  /**
   * Fires Closing USB Serial process and returns !True for openFlag if successful. Sets the IsOpen to False.
   * @return
   */

  @SimpleFunction(description = "Closes serial connection. Returns true when closed.")
  public void CloseSerial() 
  {
	boolean locClaseFLag = false;  
    Log.d(LOG_TAG, "Closing connection");
       if ((serialPort == null)||(!IsOpen()))
       {
          ErrorOccured("Error occurred on closing.  Port needs to be Initialized.");   
          form.dispatchErrorOccurredEvent(g038_otg_serial.this, "CloseSerial", 3901); //ErrorMessages.ERROR_SERIAL_NOT_INITIALIZED)
     //  openFlag = false;
     // locClaseFLag = true;
       }
       else 
      { 
		serialPort.close();
		if (serialPort == null)
		{locClaseFLag = true;}
		else
		{
			locClaseFLag = true;	
			ErrorOccured("Error occurred on closing.  Could not CLose()");
		    form.dispatchErrorOccurredEvent(g038_otg_serial.this, "SerialCloseIssue", 3901);
		    Log.d(LOG_TAG, "Closing connection");
		}
	  }
      openFlag = !locClaseFLag;
  }


  
  /**
   * 
   * @return String composed of S_FZ_G where S_ and Z_ represent axis labels an F and G 
   * represent floating point values expressed as stings.  The "_" serves as a separator.
   * <br><br>i.e 
   *   <br>A_0.003921628B_-0.0039215684  << Left joystick A: Y-Axis  B: X Axis
	   <br>F_0.003921628G_-0.0039215684  << Right joystick A: Y-Axis  B: X Axis
	   <br>C_-1.0H_-1.0                  << Left & right triggers;  
	   <br>
	   <br>Value Ranges for joystick -1 to +1 with "dead zone as shown above
	   <br>Value Ranges for trigger starts at -1 with no pressure to + 1 pressed.
	   
	   <br>
<br>as seen from JSTest-gtk
<br>Axis 0: Left JS Left- Right.  Range of values -1 to +1  A_
<br>Axes 1: Left JS Up -Down      Range of values -1 to +1  B_
<br>Axes 2: Left Trigger 	      Range of values -1 to +1  C_
<br>Axes 3: Right JS Left - Right Range of values -1 to +1  F_
<br>Axes 4: Right JS Up - Down    Range of values -1 to +1  G_
<br>Axes 5: Right Trigger         Range of values -1 to +1  H_
<br>Axes 6: dpad:  Left - Right   -1, 0, +1   See getGamepadAxesBuffer()        
<br>Axes 7: dpad  Up - Down       -1, 0, +1      for specific             

   **/
   

  @SimpleFunction(description = "TBD: Reads Axes byte package from serial.")
  public String ReadAxesBytePackage() {
    String gamepadAxes = "OK";
    return gamepadAxes;
  }
  
  
/**
	 * Purpose: Allow the user to access the most recent button inputs.
	 * 
	 * <br>Z 
   <br>Z Buttons pressed.  Z indicates null button pressed.
   <br>Z
   <br>Z
   ....  Z continues to appear until "A" button is pressed.
   <br>A
   <br>A
   ....  A continues to appear until release and then Z returns.
   
	 * 
	 * <br>XInput mode
<br>No valid button: "Z"        	 
<br>A "A" 
<br>B "B" 
<br>X "X" 
<br>Y "Y" 
<br>Left Button "L" 
<br>Right Button "R" 
<br>Back "K"  
<br>Start "S" 
<br>Logo Home "H" 
<br>Left Joystick button "W" 
<br>Right Joystick Button "V" 
<br>D-Pad Up       Axis 7 Up #11    -1
<br>D-Pad Right    Axis 6 Right  #12  +1
<br>D-Pad  Down     Axis 7 Down  #13  +1
<br>D-Pad Left      Axis 6 Left  #14  -1
	 **/



@SimpleFunction(description = "TBD: Reads Gamepad Button byte package from serial.")
public String ReadButtonBytePackage() 
{
	String gamepadButton = "Z";
    return gamepadButton;
}


/**
 * Provide the data package read from the OTG-Serial
 * @return String
 */
@SimpleFunction(description = "Reads data from serial.")
public void ReadSerialData() 
{
	  boolean success = true;
	  //String data = "oops";
	  // data = 
	  ReadSerial();
	  
      if (incomingMsg == null)
      {  
    	success = false  ;
      }
      AfterReadSerialData(success, incomingMsg);
 

}

/**
 * Provide the data package read from the OTG-Serial
 * @return String
 */
@SimpleFunction(description = "Reads data from serial.")
public String ReadSerial() 
{
 // String data = "oops";
 // int kntr4read = -1;
  //boolean success = true;
  
  if (serialPort == null)
  {
    form.dispatchErrorOccurredEvent(g038_otg_serial.this, "ReadSerial", 3901);
    //data = null;  
    //needs to be xxx_yyy_speed_A to stay with a038_iii_gamepad_otg_wifi.aia
    //            parse to string;
    incomingMsg = "100_0 50_M";   //Move slowly forwardoops: Message Receipt Error";
  } 
  return incomingMsg;  //  data;
}

/**
 * Returns True is the USB Port is open
 * @return Boolean
 */

@SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "Returns true when the Serial connection is open.")
public boolean IsOpen()
{
  return openFlag;  //mPhysicaloid.isOpened();
}


/**
 * Returns True if USB-Serial communication has been initialized.
 * @param IsIniitalized 
 */
@SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "Returns true when the Serial has been initialized.")
public boolean IsInitialized() {
  return initializationFlag; 
}


/**
 * Gets the current USB port Baud Rate
 * @return
 */
@SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "Returns the current baud rate")
public int BaudRate() {
  return 123; //200824 this.baudRate;
}


/**
 * Sets the USB port Baud Rate
 * @param baudRate
 */
@DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_INTEGER, defaultValue = "9600")
@SimpleProperty
public void BaudRate(int baudRate)
{
  ;  // do something important this.baudRate = baudRate;
}

  

/**
 * Gets the current USB byte size allowed.
 * @return
 */
  @SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "Returns the buffer size in bytes")
  public int BufferSize() {
    return this.bytes;
  }

  
  
/**
 * Sets the current USB byte size to be used.
 * @param bytes
 */
  @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_INTEGER, defaultValue = "48")  //200728 "256")
  @SimpleProperty
  public void BufferSize(int bytes) {
    this.bytes = bytes;
  //replace 200823   Log.d(LOG_TAG, "Buffer Size: " + bytes);
  }
 
  
  
  /**
   * An event declaring that a data package has been read from the OTG-Serial
   * @param success
   * @param dataPackage: As a string
   */
  
  @SimpleEvent
  public void AfterReadSerialData(boolean success, String dataPackage) {
      EventDispatcher.dispatchEvent(this, "AfterRead", success, dataPackage);
  }

  /**
   * An event declaring that a Button press has been read from the OTG-Serial
   * @param success
   * @param bytePackage
   */
  @SimpleEvent
  public void AfterButtonBytePackageRead(boolean success, String bytePackage) {
      EventDispatcher.dispatchEvent(this, "AfterRead", success, bytePackage);
  }

 /**
  * An event declaring that speed or joystick data has been read from the OTG-Serial
  * @param success
  * @param bytePackage
  */
@SimpleEvent
  public void AfterAxesBytePackageRead(boolean success, String bytePackage) {
      EventDispatcher.dispatchEvent(this, "AfterRead", success, bytePackage);
  }


/**
* Throws error message when an error is detected and handled by a 
* try-catch sequence.
* @param String error message
* 
*/

@SimpleEvent(description = "Throws error messages upon an error.")
public void ErrorOccured(String error) {
    EventDispatcher.dispatchEvent(this, "ErrorOccured", error);
}

  

}// **** end of class
