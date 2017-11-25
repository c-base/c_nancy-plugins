using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Plugins {
  public class UCCNCplugin { // Class name must be UCCNCplugin to work!                             
    [DllImport("msgflo.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool mqttConnect([MarshalAs(UnmanagedType.LPStr)]String pBrokerHostName,
        [MarshalAs(UnmanagedType.LPStr)]String pClientId);

    [DllImport("msgflo.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool mqttDisconnect();

    [DllImport("msgflo.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool mqttPublish([MarshalAs(UnmanagedType.LPStr)]String pTopic,
        [MarshalAs(UnmanagedType.LPStr)]String pPayload, int len, int qos, bool retain);

    bool isFirstCycle = true;
    public Plugininterface.Entry UC;
    PluginForm myform;
    public bool loopstop = false;
    public bool loopworking = false;
    public int hClient_ = 0;
    public String brokerHostname_ = "tcp://c-beam:1883";
    public String topic_ = "werkstatt/c_nancy/";

    public UCCNCplugin() {

    }

    public class UcncMsgFloStatus {
      public bool running { get; set; }
    }

    private void onFirstCycle() {
      try {
        mqttConnect(brokerHostname_, "c_nancy");
        mqttPublish(topic_ + "running", "true", 4, 1, true);
      }
      catch(Exception) {
        MessageBox.Show("Exception in msg-flow pluging!", "Error!");
      }
    }

    public long lastTick = DateTime.Now.Ticks;
    public long lastTick2 = DateTime.Now.Ticks;

    private void onTick() {
      long tick = DateTime.Now.Ticks;
      long diffSeconds = (tick - lastTick) / 10000000;
      long diffSeconds2 = (tick - lastTick2) / 10000000;

      if (diffSeconds2 == 1) {
        var x = UC.Getfield(true, 226);
        var y = UC.Getfield(true, 227);
        var z = UC.Getfield(true, 228);
        var a = UC.Getfield(true, 229);
        var b = UC.Getfield(true, 230);
        var c = UC.Getfield(true, 231);

        String status = String.Format(@"{{""X"": {0}, ""Y"": {1}, ""Z"": {2}," +
                                      @"  ""A"": {3}, ""B"": {4}, ""C"": {5}}}",
                                      x, y, z, a, b, c);

        mqttPublish(topic_ + "status", status, status.Length, 1, false);
        lastTick2 = DateTime.Now.Ticks;
      }

      if (diffSeconds == 60) {
        String discovery = @"{""protocol"": ""discovery"", ""command"": ""participant"", ""payload"":
                           { ""component"": ""c-base/c_nancy"", ""label"": ""CNC mill status"",
                           ""icon"": ""scissors"", ""inports"": [],
                           ""outports"": [{""id"": ""running"", ""type"": ""boolean"", ""queue"": ""werkstatt/c_nancy/running""}], ""role"": ""c_nancy"", ""id"": ""c_nancy""}}";

        mqttPublish("fbp", discovery, discovery.Length, 1, true);
        lastTick = DateTime.Now.Ticks;
      }

      
    }
    // Called when the plugin is initialised.
    // The parameter is the Plugin interface object which contains all functions prototypes for calls and callbacks.
    public void Init_event(Plugininterface.Entry UC) {
      this.UC = UC;
      myform = new PluginForm(this);
    }

    // Called when the plugin is loaded, the author of the plugin should set the details of the plugin here.
    public Plugininterface.Entry.Pluginproperties Getproperties_event(Plugininterface.Entry.Pluginproperties Properties) {
      Properties.author = "coon@c-base.org";
      Properties.pluginname = "msg-flo";
      Properties.pluginversion = "1.0000";
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
        mqttPublish(topic_ + "running", "false", 5, 1, true);
        mqttDisconnect();

        myform.Close();
      }
      catch (Exception) { }
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
      catch (Exception) {
          MessageBox.Show("Exception in msg-flow pluging!", "Error!");
      }

      loopworking = false;
      //Console.WriteLine("" + Convert.ToInt32('A'));
    }

    //This is a direct function call addressed to this plugin dll
    //The function can be called by macros or by another plugin
    //The passed parameter is an object and the return value is also an object
    public object Informplugin_event(object Message) {
      if (!(myform == null || myform.IsDisposed)) {
        if (Message is string) {
          string receivedstr = Message as string;
          MessageBox.Show(this.myform, "Informplugin message received by Plugintest! Message was: " + receivedstr);
        }
      }

      string returnstr = "Return string by Plugintest";
      return (object)returnstr;
    }

    //This is a function call made to all plugin dll files
    //The function can be called by macros or by another plugin
    //The passed parameter is an object and there is no return value
    public void Informplugins_event(object Message) {
      if (!(myform == null || myform.IsDisposed)) {
        string receivedstr = Message as string;
        MessageBox.Show(this.myform, "Informplugins message received by Plugintest! Message was: " + receivedstr);
      }
    }

    //Called when the user presses a button on the UCCNC GUI or if a Callbutton function is executed.
    //The int buttonnumber parameter is the ID of the caller button.
    // The bool onscreen parameter is true if the button was pressed on the GUI and is false if the Callbutton function was called.
    public void Buttonpress_event(int buttonnumber, bool onscreen) {
      if (onscreen) {
        if (buttonnumber == 128) {
          // TODO: implement
        }
      }
    }

    //Called when the user clicks and enters a Textfield on the screen
    //The labelnumber parameter is the ID of the accessed Textfield
    //The bool Ismainscreen parameter is true is the Textfield is on the main screen and false if it is on the jog screen
    public void Textfieldclick_event(int labelnumber, bool Ismainscreen) {
      if (Ismainscreen) {
        if (labelnumber == 1000) {
          // TODO: implement
        }
      }
    }

    //Called when the user enters text into the Textfield and it gets validated
    //The labelnumber parameter is the ID of the accessed Textfield
    //The bool Ismainscreen parameter is true is the Textfield is on the main screen and false if it is on the jog screen.
    //The text parameter is the text entered and validated by the user
    public void Textfieldtexttyped_event(int labelnumber, bool Ismainscreen, string text) {
      if (Ismainscreen) {
        if (labelnumber == 1000) {
          // TODO: implement
        }
      }
    }
  }
}
