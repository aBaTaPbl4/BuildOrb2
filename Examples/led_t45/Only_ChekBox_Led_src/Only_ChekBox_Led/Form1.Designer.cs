namespace WindowsFormsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// Требуется переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Обязательный метод для поддержки конструктора - не изменяйте
        /// содержимое данного метода при помощи редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.checkBoxLed = new System.Windows.Forms.CheckBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // checkBoxLed
            // 
            this.checkBoxLed.AutoSize = true;
            this.checkBoxLed.Location = new System.Drawing.Point(25, 10);
            this.checkBoxLed.Margin = new System.Windows.Forms.Padding(0);
            this.checkBoxLed.Name = "checkBoxLed";
            this.checkBoxLed.Size = new System.Drawing.Size(72, 17);
            this.checkBoxLed.TabIndex = 0;
            this.checkBoxLed.Text = "Led is on.";
            this.checkBoxLed.UseVisualStyleBackColor = true;
            this.checkBoxLed.CheckedChanged += new System.EventHandler(this.checkBoxLed_CheckedChanged);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 300;
            this.timer1.Tick += new System.EventHandler(this.Form1_Load);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(127, 34);
            this.Controls.Add(this.checkBoxLed);
            this.MaximumSize = new System.Drawing.Size(135, 65);
            this.MinimumSize = new System.Drawing.Size(135, 65);
            this.Name = "Form1";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Led";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkBoxLed;
        private System.Windows.Forms.Timer timer1;
    }
}

