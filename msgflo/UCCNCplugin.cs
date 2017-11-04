using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Plugins {
  public class UCCNCplugin { // Class name must be UCCNCplugin to work!
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public unsafe struct MQTTClient_connOptions {
      public char struct_id0;
      public char struct_id1;
      public char struct_id2;
      public char struct_id3;
      public int struct_version;
      public int keepAliveInterval;
      public int cleansession;
      public int reliable;
      public void* will;
      public void* username;
      public void* password;
      public int connectTimeout;
      public int retryInterval;
      public void* ssl;
      public int serverURIcount;
      public void* serverURIs;
      public int MQTTVersion;
      public void* returnedServerURI;
      public int returnedMQTTVersion;
      public int returnedSessionPresent;
      public int binarypwLen;
      public void* binarypwData;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
    public unsafe struct MQTTClient_message {
      public char struct_id0;
      public char struct_id1;
      public char struct_id2;
      public char struct_id3;
      public int struct_version;
      public int payloadlen;
      public void* payload;
      public int qos;
      public int retained;
      public int dup;
      public int msgid;
    }

    [DllImport("paho-mqtt3c.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern unsafe int MQTTClient_create(ref int handle, StringBuilder serverURI,
        StringBuilder clientId, int persistence_type, void* persistence_context);

    [DllImport("paho-mqtt3c.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern unsafe int MQTTClient_connect(int handle, ref MQTTClient_connOptions options);

    [DllImport("paho-mqtt3c.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern unsafe int MQTTClient_publishMessage(int handle, StringBuilder topicName,
        ref MQTTClient_message message, ref int deliveryToken);

    [DllImport("paho-mqtt3c.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern unsafe int MQTTClient_disconnect(int handle, int timeout);

    [DllImport("paho-mqtt3c.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern unsafe void MQTTClient_destroy(ref int handle);

    static unsafe int connect(ref int hClient, String hostname) {
      MQTTClient_connOptions opts = new MQTTClient_connOptions();

      // init
      opts.struct_id0 = 'M';
      opts.struct_id1 = 'Q';
      opts.struct_id2 = 'T';
      opts.struct_id3 = 'C';
      opts.struct_version = 5;
      opts.keepAliveInterval = 60;
      opts.cleansession = 1;
      opts.reliable = 1;
      opts.will = null;
      opts.username = null;
      opts.password = null;
      opts.connectTimeout = 30;
      opts.retryInterval = 20;
      opts.ssl = null;
      opts.serverURIcount = 0;
      opts.serverURIs = null;
      opts.MQTTVersion = 0;
      opts.returnedServerURI = null;
      opts.returnedMQTTVersion = 0;
      opts.returnedSessionPresent = 0;
      opts.binarypwLen = 0;
      opts.binarypwData = null;
      // -

      opts.keepAliveInterval = 20;
      opts.cleansession = 1;

      StringBuilder clientId = new StringBuilder("c_nancy");
      StringBuilder uri = new StringBuilder(hostname);

      MQTTClient_create(ref hClient, uri, clientId, 1, null);
      return MQTTClient_connect(hClient, ref opts);
    }

    static unsafe int send(int hClient, string topic, String payload) {
      MQTTClient_message msg = new MQTTClient_message();
      // init
      msg.struct_id0 = 'M';
      msg.struct_id1 = 'Q';
      msg.struct_id2 = 'T';
      msg.struct_id3 = 'M';
      msg.struct_version = 0;
      msg.payloadlen = 0;
      msg.payload = null;
      msg.qos = 0;
      msg.retained = 0;
      msg.dup = 0;
      msg.msgid = 0;
      // -

      byte* p = stackalloc byte[payload.Length + 1];

      for (int i = 0; i < payload.Length; i++)
        p[i] = (byte)payload[i];

      p[payload.Length] = 0;

      msg.payload = p;
      msg.payloadlen = payload.Length;

      msg.qos = 1;
      msg.retained = 0;
      int deliveredToken = 0;

      StringBuilder tmp = new StringBuilder(256);
      tmp.Append(topic);

      return MQTTClient_publishMessage(hClient, tmp, ref msg, ref deliveredToken);
    }

    bool isFirstCycle = true;
    public Plugininterface.Entry UC;
    PluginForm myform;
    public bool loopstop = false;
    public bool loopworking = false;

    public UCCNCplugin() {

    }

    private void onFirstCycle() {
      int error = 0;
      int hClient = 0;

      error = connect(ref hClient, "tcp://iot.eclipse.org:1883");
      error = send(hClient, "c-base/c_cancy", "Hello from c_nancy!");
      error = MQTTClient_disconnect(hClient, 10000);
      MQTTClient_destroy(ref hClient);
    }

    private void onTick() {
      myform.label1.Text = "X: " + UC.Getfield(true, 226);
      myform.label2.Text = "Y: " + UC.Getfield(true, 227);
      myform.label3.Text = "Z: " + UC.Getfield(true, 228);
      myform.label4.Text = "A: " + UC.Getfield(true, 229);
      myform.label5.Text = "Set feed: " + UC.Getfield(true, 867);
      myform.label6.Text = "Act feed: " + UC.Getfield(true, 868);
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
