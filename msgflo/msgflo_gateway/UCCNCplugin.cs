using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Plugins {
  public class UCCNCplugin { // Class name must be UCCNCplugin to work!
    public const string cppDll = "cpp\\msgflo.dll";

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool onFirstCycle();

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool onTick();

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool onShutdown();

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern void buttonpress_event(int buttonnumber, bool onscreen);

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern void textfieldclick_event(int labelnumber, bool Ismainscreen);

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern void textfieldtexttyped_event(int labelnumber, bool Ismainscreen, StringBuilder text);

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern void getproperties_event(StringBuilder author, StringBuilder pluginName,
        StringBuilder pluginVersion);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate bool IsMovingCallBack();
    public IsMovingCallBack pIsMoving; // Ensure it doesn't get garbage collected
    private unsafe bool IsMovingHandler() {
      return UC.IsMoving();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public unsafe delegate void GetFieldCallBack(byte* pResult, int resultBufLen, bool isAS3, int fieldnumber);
    public GetFieldCallBack pGetField; // Ensure it doesn't get garbage collected
    private unsafe void GetFieldHandler(byte* pResult, int resultBufLen, bool isAS3, int fieldnumber) {
      string result = UC.Getfield(isAS3, fieldnumber);

      // TOOD: how to do the string copy right!?
      int len = result.Length < resultBufLen + 1 ? result.Length : resultBufLen - 1;

      for (int i = 0; i < len; i++)
        pResult[i] = (byte)result[i];

      pResult[len] = (byte)'\0';
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int GetFieldIntCallBack(bool isAS3, int fieldnumber);
    public GetFieldIntCallBack pGetFieldInt; // Ensure it doesn't get garbage collected
    private int GetFieldIntHandler(bool isAS3, int fieldnumber) {
        return UC.Getfieldint(isAS3, fieldnumber);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetFieldDoubleCallBack(bool isAS3, int fieldnumber);
    public GetFieldDoubleCallBack pGetFieldDouble; // Ensure it doesn't get garbage collected
    private double GetFieldDoubleHandler(bool isAS3, int fieldnumber) {
      return UC.Getfielddouble(isAS3, fieldnumber);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate bool GetLedCallBack(int ledNumber);
    public GetLedCallBack pGetLed; // Ensure it doesn't get garbage collected
    private bool GetLedHandler(int ledNumber) {
      return UC.GetLED(ledNumber);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetXposCallBack();
    public GetXposCallBack pGetXpos; // Ensure it doesn't get garbage collected
    private double GetXposHandler() {
      return UC.GetXpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetYposCallBack();
    public GetYposCallBack pGetYpos; // Ensure it doesn't get garbage collected
    private double GetYposHandler() {
      return UC.GetYpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetZposCallBack();
    public GetZposCallBack pGetZpos; // Ensure it doesn't get garbage collected
    private double GetZposHandler() {
      return UC.GetZpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetAposCallBack();
    public GetAposCallBack pGetApos; // Ensure it doesn't get garbage collected
    private double GetAposHandler() {
      return UC.GetApos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetBposCallBack();
    public GetBposCallBack pGetBpos; // Ensure it doesn't get garbage collected
    private double GetBposHandler() {
      return UC.GetBpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetCposCallBack();
    public GetCposCallBack pGetCpos; // Ensure it doesn't get garbage collected
    private double GetCposHandler() {
      return UC.GetCpos();
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct PluginInterfaceEntry {
      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetFieldCallBack pGetField;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetFieldIntCallBack pGetFieldInt;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetFieldDoubleCallBack pGetFieldDouble;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetLedCallBack pGetLed;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public IsMovingCallBack pIsMoving;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetXposCallBack pGetXpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetYposCallBack pGetYpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetZposCallBack pGetZpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetAposCallBack pGetApos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetBposCallBack pGetBpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetCposCallBack pGetCpos;
    }

    [DllImport(cppDll, CallingConvention = CallingConvention.Cdecl)]
    public static extern void setCallBacks(PluginInterfaceEntry pInterface);

    public Plugininterface.Entry UC;
    PluginForm myform;
    bool isFirstCycle = true;
    public bool loopstop = false;
    public bool loopworking = false;

    public unsafe UCCNCplugin() {
      // Use instance variables to ensure the pointers doesn't get garbage collected:
      pGetField       = new GetFieldCallBack(GetFieldHandler);
      pGetFieldInt    = new GetFieldIntCallBack(GetFieldIntHandler);
      pGetFieldDouble = new GetFieldDoubleCallBack(GetFieldDoubleHandler);
      pGetLed         = new GetLedCallBack(GetLedHandler);
      pIsMoving       = new IsMovingCallBack(IsMovingHandler);
      pGetXpos        = new GetXposCallBack(GetXposHandler);
      pGetYpos        = new GetYposCallBack(GetYposHandler);
      pGetZpos        = new GetZposCallBack(GetZposHandler);
      pGetApos        = new GetAposCallBack(GetAposHandler);
      pGetBpos        = new GetBposCallBack(GetBposHandler);
      pGetCpos        = new GetCposCallBack(GetCposHandler);
      // --

      PluginInterfaceEntry uc = new PluginInterfaceEntry();
      uc.pGetField       = pGetField;
      uc.pGetFieldInt    = pGetFieldInt;
      uc.pGetFieldDouble = pGetFieldDouble;
      uc.pGetLed         = pGetLed;
      uc.pIsMoving       = pIsMoving;
      uc.pGetXpos        = pGetXpos;
      uc.pGetYpos        = pGetYpos;
      uc.pGetZpos        = pGetZpos;
      uc.pGetApos        = pGetApos;
      uc.pGetBpos        = pGetBpos;
      uc.pGetCpos        = pGetCpos;

      setCallBacks(uc);
    }

    // Called when the plugin is initialised.
    // The parameter is the Plugin interface object which contains all functions prototypes for calls and callbacks.
    public void Init_event(Plugininterface.Entry UC) {
      this.UC = UC;
      myform = new PluginForm(this);
    }

    // Called when the plugin is loaded, the author of the plugin should set the details of the plugin here.
    public Plugininterface.Entry.Pluginproperties Getproperties_event(Plugininterface.Entry.Pluginproperties Properties) {
      StringBuilder author = new StringBuilder(256);
      StringBuilder pluginName = new StringBuilder(256);
      StringBuilder pluginVersion = new StringBuilder(256);

      getproperties_event(author, pluginName, pluginVersion);

      Properties.author = author.ToString();
      Properties.pluginname = pluginName.ToString();;
      Properties.pluginversion = pluginVersion.ToString();
      return Properties;
    }

    // Called from UCCNC when the user presses the Configuration button in the Plugin configuration menu.
    // Typically the plugin configuration window is shown to the user.
    public void Configure_event() {
      ConfigForm cform = new ConfigForm();
      cform.ShowDialog();
    }

    // Called from UCCNC when the plugin is loaded and started.
    public void Startup_event() {
      if (myform.IsDisposed)
        myform = new PluginForm(this);

      myform.Show();
    }

    // Called when the Pluginshowup(string Pluginfilename); function is executed in the UCCNC.
    public void Showup_event() {
      if (myform.IsDisposed)
        myform = new PluginForm(this);

      myform.Show();
      myform.BringToFront();
    }

    // Called when the UCCNC software is closing.
    public void Shutdown_event() {
      try {
        loopstop = true;
        onShutdown();
        myform.Close();
      }
      catch (Exception) {
        MessageBox.Show("Exception in msg-flow pluging!", "Error in Shutdown_event");
      }
    }

    // Called in a loop with a 25Hz interval.
    public void Loop_event()  {
      if (loopstop)
        return;

      loopworking = true;

      if (myform == null || myform.IsDisposed)
        return;

      if (isFirstCycle) {
        isFirstCycle = false;
        onFirstCycle();
      }

      try {
        onTick();
      }
      catch (Exception e) {
        MessageBox.Show(String.Format("Exception in msg-flow pluging!\n {0}", e.StackTrace), "Error in Loop_event");
      }

      loopworking = false;
    }

    //This is a direct function call addressed to this plugin dll
    //The function can be called by macros or by another plugin
    //The passed parameter is an object and the return value is also an object
    public object Informplugin_event(object Message) {
      if (!(myform == null || myform.IsDisposed)) {
        if (Message is string) {
          string receivedstr = Message as string;
          MessageBox.Show(this.myform, "Informplugin message received by msgflo plugin! Message was: " + receivedstr);
        }
      }

      string returnstr = "Return string by msgflo";
      return (object)returnstr;
    }

    //This is a function call made to all plugin dll files
    //The function can be called by macros or by another plugin
    //The passed parameter is an object and there is no return value
    public void Informplugins_event(object Message) {
      if (!(myform == null || myform.IsDisposed)) {
        string receivedstr = Message as string;
        MessageBox.Show(this.myform, "Informplugins message received by msgflo! Message was: " + receivedstr);
      }
    }

    //Called when the user presses a button on the UCCNC GUI or if a Callbutton function is executed.
    //The int buttonnumber parameter is the ID of the caller button.
    // The bool onscreen parameter is true if the button was pressed on the GUI and is false if the Callbutton function was called.
    public void Buttonpress_event(int buttonnumber, bool onscreen) {
      buttonpress_event(buttonnumber, onscreen);
    }

    //Called when the user clicks and enters a Textfield on the screen
    //The labelnumber parameter is the ID of the accessed Textfield
    //The bool Ismainscreen parameter is true is the Textfield is on the main screen and false if it is on the jog screen
    public void Textfieldclick_event(int labelnumber, bool Ismainscreen) {
      textfieldclick_event(labelnumber, Ismainscreen);
    }

    //Called when the user enters text into the Textfield and it gets validated
    //The labelnumber parameter is the ID of the accessed Textfield
    //The bool Ismainscreen parameter is true is the Textfield is on the main screen and false if it is on the jog screen.
    //The text parameter is the text entered and validated by the user
    public void Textfieldtexttyped_event(int labelnumber, bool Ismainscreen, string text) {
      StringBuilder sbText = new StringBuilder(text);
      textfieldtexttyped_event(labelnumber, Ismainscreen, sbText);
    }
  }
}
