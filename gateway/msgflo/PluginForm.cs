using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace Plugins
{
    public partial class PluginForm : Form
    {
        private Plugininterface.Entry UC;
        UCCNCplugin Pluginmain;
        bool mustclose = false;

        public PluginForm(UCCNCplugin Pluginmain)
        {
            this.UC = Pluginmain.UC;
            this.Pluginmain = Pluginmain;
            InitializeComponent();
        }

        private void PluginForm_Load(object sender, EventArgs e)
        {
            CheckForIllegalCrossThreadCalls = false; 
        }

        private void jogXplusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(147);            
        }

        private void jogXplusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(229);
        }

        private void jogXminusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(148);
        }

        private void jogXminusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(230);
        }

        private void jogYplusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(149);
        }

        private void jogYplusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(231);
        }

        private void jogYminusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(150);
        }

        private void jogYminusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(232);
        }

        private void jogZplusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(151);
        }

        private void jogZplusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(233);
        }

        private void jogZminusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(152);
        }

        private void jogZminusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(234);
        }

        private void jogAplusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(153);
        }

        private void jogAplusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(235);
        }

        private void jogAminusbutton_MouseDown(object sender, MouseEventArgs e)
        {
            UC.Callbutton(154);
        }

        private void jogAminusbutton_MouseUp(object sender, MouseEventArgs e)
        {
            UC.Callbutton(236);
        }

        private void PluginForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //Do not close the form when the red X button is pressed
            //But start a Thread which will stop the Loop call from the UCCNC
            //to prevent the form closing while there is a GUI update in the Loop event
            if (!mustclose)
            {
                e.Cancel = true;
                Thread closethr = new Thread(new ThreadStart(Closeform));
                closethr.Start();
            }
            else
            {
                //Form is closing here...
            }
        }

        public void Closeform()
        {
            //Stop the Loop event to update the GUI
            Pluginmain.loopstop = true;
            //Wait until the loop exited
            while (Pluginmain.loopworking)
            {
                Thread.Sleep(10);
            }
            //Set the mustclose variable to true and call the .Close() function to close the Form
            mustclose = true;
            this.Close();
        }
    }
}
