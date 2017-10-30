namespace Plugins
{
    partial class PluginForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.jogXplusbutton = new System.Windows.Forms.Button();
            this.jogXminusbutton = new System.Windows.Forms.Button();
            this.jogYminusbutton = new System.Windows.Forms.Button();
            this.jogZminusbutton = new System.Windows.Forms.Button();
            this.jogAminusbutton = new System.Windows.Forms.Button();
            this.jogYplusbutton = new System.Windows.Forms.Button();
            this.jogZplusbutton = new System.Windows.Forms.Button();
            this.jogAplusbutton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label1.Location = new System.Drawing.Point(13, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "labelXpos";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label2.Location = new System.Drawing.Point(13, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(108, 25);
            this.label2.TabIndex = 1;
            this.label2.Text = "labelYpos";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label3.Location = new System.Drawing.Point(13, 93);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(106, 25);
            this.label3.TabIndex = 2;
            this.label3.Text = "labelZpos";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label4.Location = new System.Drawing.Point(13, 129);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(107, 25);
            this.label4.TabIndex = 3;
            this.label4.Text = "labelApos";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label5.Location = new System.Drawing.Point(13, 203);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(132, 25);
            this.label5.TabIndex = 4;
            this.label5.Text = "labelSetfeed";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label6.Location = new System.Drawing.Point(13, 167);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(131, 25);
            this.label6.TabIndex = 5;
            this.label6.Text = "labelActfeed";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // jogXplusbutton
            // 
            this.jogXplusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogXplusbutton.Location = new System.Drawing.Point(255, 20);
            this.jogXplusbutton.Name = "jogXplusbutton";
            this.jogXplusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogXplusbutton.TabIndex = 6;
            this.jogXplusbutton.Text = "X+";
            this.jogXplusbutton.UseVisualStyleBackColor = true;            
            this.jogXplusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogXplusbutton_MouseDown);
            this.jogXplusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogXplusbutton_MouseUp);
            // 
            // jogXminusbutton
            // 
            this.jogXminusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogXminusbutton.Location = new System.Drawing.Point(209, 20);
            this.jogXminusbutton.Name = "jogXminusbutton";
            this.jogXminusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogXminusbutton.TabIndex = 7;
            this.jogXminusbutton.Text = "X-";
            this.jogXminusbutton.UseVisualStyleBackColor = true;
            this.jogXminusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogXminusbutton_MouseDown);
            this.jogXminusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogXminusbutton_MouseUp);
            // 
            // jogYminusbutton
            // 
            this.jogYminusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogYminusbutton.Location = new System.Drawing.Point(209, 56);
            this.jogYminusbutton.Name = "jogYminusbutton";
            this.jogYminusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogYminusbutton.TabIndex = 8;
            this.jogYminusbutton.Text = "Y-";
            this.jogYminusbutton.UseVisualStyleBackColor = true;
            this.jogYminusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogYminusbutton_MouseDown);
            this.jogYminusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogYminusbutton_MouseUp);
            // 
            // jogZminusbutton
            // 
            this.jogZminusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogZminusbutton.Location = new System.Drawing.Point(209, 92);
            this.jogZminusbutton.Name = "jogZminusbutton";
            this.jogZminusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogZminusbutton.TabIndex = 9;
            this.jogZminusbutton.Text = "Z-";
            this.jogZminusbutton.UseVisualStyleBackColor = true;
            this.jogZminusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogZminusbutton_MouseDown);
            this.jogZminusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogZminusbutton_MouseUp);
            // 
            // jogAminusbutton
            // 
            this.jogAminusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogAminusbutton.Location = new System.Drawing.Point(209, 128);
            this.jogAminusbutton.Name = "jogAminusbutton";
            this.jogAminusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogAminusbutton.TabIndex = 10;
            this.jogAminusbutton.Text = "A-";
            this.jogAminusbutton.UseVisualStyleBackColor = true;
            this.jogAminusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogAminusbutton_MouseDown);
            this.jogAminusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogAminusbutton_MouseUp);
            // 
            // jogYplusbutton
            // 
            this.jogYplusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogYplusbutton.Location = new System.Drawing.Point(255, 56);
            this.jogYplusbutton.Name = "jogYplusbutton";
            this.jogYplusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogYplusbutton.TabIndex = 11;
            this.jogYplusbutton.Text = "Y+";
            this.jogYplusbutton.UseVisualStyleBackColor = true;
            this.jogYplusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogYplusbutton_MouseDown);
            this.jogYplusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogYplusbutton_MouseUp);
            // 
            // jogZplusbutton
            // 
            this.jogZplusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogZplusbutton.Location = new System.Drawing.Point(255, 92);
            this.jogZplusbutton.Name = "jogZplusbutton";
            this.jogZplusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogZplusbutton.TabIndex = 12;
            this.jogZplusbutton.Text = "Z+";
            this.jogZplusbutton.UseVisualStyleBackColor = true;
            this.jogZplusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogZplusbutton_MouseDown);
            this.jogZplusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogZplusbutton_MouseUp);
            // 
            // jogAplusbutton
            // 
            this.jogAplusbutton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.jogAplusbutton.Location = new System.Drawing.Point(255, 128);
            this.jogAplusbutton.Name = "jogAplusbutton";
            this.jogAplusbutton.Size = new System.Drawing.Size(40, 30);
            this.jogAplusbutton.TabIndex = 13;
            this.jogAplusbutton.Text = "A+";
            this.jogAplusbutton.UseVisualStyleBackColor = true;
            this.jogAplusbutton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.jogAplusbutton_MouseDown);
            this.jogAplusbutton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.jogAplusbutton_MouseUp);
            // 
            // PluginForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(301, 235);
            this.Controls.Add(this.jogAplusbutton);
            this.Controls.Add(this.jogZplusbutton);
            this.Controls.Add(this.jogYplusbutton);
            this.Controls.Add(this.jogAminusbutton);
            this.Controls.Add(this.jogZminusbutton);
            this.Controls.Add(this.jogYminusbutton);
            this.Controls.Add(this.jogXminusbutton);
            this.Controls.Add(this.jogXplusbutton);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "PluginForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "PluginForm";
            this.TopMost = true;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PluginForm_FormClosing);
            this.Load += new System.EventHandler(this.PluginForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.Label label1;
        public System.Windows.Forms.Label label2;
        public System.Windows.Forms.Label label3;
        public System.Windows.Forms.Label label4;
        public System.Windows.Forms.Label label5;
        public System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button jogXplusbutton;
        private System.Windows.Forms.Button jogXminusbutton;
        private System.Windows.Forms.Button jogYminusbutton;
        private System.Windows.Forms.Button jogZminusbutton;
        private System.Windows.Forms.Button jogAminusbutton;
        private System.Windows.Forms.Button jogYplusbutton;
        private System.Windows.Forms.Button jogZplusbutton;
        private System.Windows.Forms.Button jogAplusbutton;

    }
}