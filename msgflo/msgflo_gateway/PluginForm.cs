using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace Plugins {
  public partial class PluginForm : Form {
    private Plugininterface.Entry UC;
    UCCNCplugin Pluginmain;
    bool mustclose = false;

    public PluginForm(UCCNCplugin Pluginmain) {
      this.UC = Pluginmain.UC;
      this.Pluginmain = Pluginmain;
      InitializeComponent();
    }

    private void PluginForm_Load(object sender, EventArgs e) {
      CheckForIllegalCrossThreadCalls = false;
      setButtons();
    }

    private void PluginForm_FormClosing(object sender, FormClosingEventArgs e) {
      // Do not close the form when the red X button is pressed
      // But start a Thread which will stop the Loop call from the UCCNC
      // to prevent the form closing while there is a GUI update in the Loop event
      if (!mustclose) {
        e.Cancel = true;
        Thread closethr = new Thread(new ThreadStart(Closeform));
        closethr.Start();
      }
      else {
        // Form is closing here...
      }
    }

    public void Closeform() {
      // Stop the Loop event to update the GUI
      Pluginmain.loopstop = true;

      // Wait until the loop exited
      while (Pluginmain.loopworking) {
        Thread.Sleep(10);
      }

      // Set the mustclose variable to true and call the .Close() function to close the Form
      mustclose = true;
      this.Close();
    }

    private void setButtons() {
      if (Pluginmain.cppDll.IsLoaded()) {
        button_load.Enabled = false;
        button_unload.Enabled = true;
      }
      else {
        button_load.Enabled = true;
        button_unload.Enabled = false;
      }
    }

    private void button_load_Click(object sender, EventArgs e) {
      // TODO: Do not load dll directly. Mark for load and do loading in onTick() instead.
      Pluginmain.isFirstCycle = true;
      Pluginmain.cppDll.Load();
      setButtons();
    }

    private void button_unload_Click(object sender, EventArgs e) {
      // TODO: Do not unload dll directly. Mark for load and do unloading in onTick() instead.
      //       Direct unloading may lead to a crash, when beeing inside the dll while unloading.

      Pluginmain.cppDll.Unload();
      setButtons();
    }
  }
}
