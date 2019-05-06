namespace DivergenceClock
{
    partial class MainForm
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置 Managed 資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器
        /// 修改這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnAutoSetTime = new System.Windows.Forms.Button();
            this.btnTime = new System.Windows.Forms.Button();
            this.btnDate = new System.Windows.Forms.Button();
            this.btnStopwatch = new System.Windows.Forms.Button();
            this.btnWorldLine = new System.Windows.Forms.Button();
            this.btnMarquee = new System.Windows.Forms.Button();
            this.btnTemperature = new System.Windows.Forms.Button();
            this.btnButton1 = new System.Windows.Forms.Button();
            this.btnButton2 = new System.Windows.Forms.Button();
            this.btnButton3 = new System.Windows.Forms.Button();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.button1 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.button3 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.timerAutoSetTime = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            this.SuspendLayout();
            // 
            // btnAutoSetTime
            // 
            this.btnAutoSetTime.Location = new System.Drawing.Point(197, 27);
            this.btnAutoSetTime.Name = "btnAutoSetTime";
            this.btnAutoSetTime.Size = new System.Drawing.Size(75, 23);
            this.btnAutoSetTime.TabIndex = 0;
            this.btnAutoSetTime.Text = "AutoSetTime";
            this.btnAutoSetTime.UseVisualStyleBackColor = true;
            this.btnAutoSetTime.Click += new System.EventHandler(this.btnAutoSetTime_Click);
            // 
            // btnTime
            // 
            this.btnTime.Location = new System.Drawing.Point(28, 27);
            this.btnTime.Name = "btnTime";
            this.btnTime.Size = new System.Drawing.Size(75, 23);
            this.btnTime.TabIndex = 1;
            this.btnTime.Text = "Time";
            this.btnTime.UseVisualStyleBackColor = true;
            this.btnTime.Click += new System.EventHandler(this.btnTime_Click);
            // 
            // btnDate
            // 
            this.btnDate.Location = new System.Drawing.Point(109, 27);
            this.btnDate.Name = "btnDate";
            this.btnDate.Size = new System.Drawing.Size(75, 23);
            this.btnDate.TabIndex = 1;
            this.btnDate.Text = "Date";
            this.btnDate.UseVisualStyleBackColor = true;
            this.btnDate.Click += new System.EventHandler(this.btnDate_Click);
            // 
            // btnStopwatch
            // 
            this.btnStopwatch.Location = new System.Drawing.Point(28, 74);
            this.btnStopwatch.Name = "btnStopwatch";
            this.btnStopwatch.Size = new System.Drawing.Size(75, 23);
            this.btnStopwatch.TabIndex = 1;
            this.btnStopwatch.Text = "Stopwatch";
            this.btnStopwatch.UseVisualStyleBackColor = true;
            this.btnStopwatch.Click += new System.EventHandler(this.btnStopwatch_Click);
            // 
            // btnWorldLine
            // 
            this.btnWorldLine.Location = new System.Drawing.Point(109, 74);
            this.btnWorldLine.Name = "btnWorldLine";
            this.btnWorldLine.Size = new System.Drawing.Size(75, 23);
            this.btnWorldLine.TabIndex = 1;
            this.btnWorldLine.Text = "WorldLine";
            this.btnWorldLine.UseVisualStyleBackColor = true;
            this.btnWorldLine.Click += new System.EventHandler(this.btnWorldLine_Click);
            // 
            // btnMarquee
            // 
            this.btnMarquee.Location = new System.Drawing.Point(28, 112);
            this.btnMarquee.Name = "btnMarquee";
            this.btnMarquee.Size = new System.Drawing.Size(75, 23);
            this.btnMarquee.TabIndex = 1;
            this.btnMarquee.Text = "Marquee";
            this.btnMarquee.UseVisualStyleBackColor = true;
            this.btnMarquee.Click += new System.EventHandler(this.btnMarquee_Click);
            // 
            // btnTemperature
            // 
            this.btnTemperature.Location = new System.Drawing.Point(109, 112);
            this.btnTemperature.Name = "btnTemperature";
            this.btnTemperature.Size = new System.Drawing.Size(75, 23);
            this.btnTemperature.TabIndex = 1;
            this.btnTemperature.Text = "Temperature";
            this.btnTemperature.UseVisualStyleBackColor = true;
            this.btnTemperature.Click += new System.EventHandler(this.btnTemperature_Click);
            // 
            // btnButton1
            // 
            this.btnButton1.Location = new System.Drawing.Point(28, 201);
            this.btnButton1.Name = "btnButton1";
            this.btnButton1.Size = new System.Drawing.Size(75, 23);
            this.btnButton1.TabIndex = 2;
            this.btnButton1.Text = "button1";
            this.btnButton1.UseVisualStyleBackColor = true;
            this.btnButton1.Click += new System.EventHandler(this.btnButton1_Click);
            // 
            // btnButton2
            // 
            this.btnButton2.Location = new System.Drawing.Point(109, 201);
            this.btnButton2.Name = "btnButton2";
            this.btnButton2.Size = new System.Drawing.Size(75, 23);
            this.btnButton2.TabIndex = 2;
            this.btnButton2.Text = "button2";
            this.btnButton2.UseVisualStyleBackColor = true;
            this.btnButton2.Click += new System.EventHandler(this.btnButton2_Click);
            // 
            // btnButton3
            // 
            this.btnButton3.Location = new System.Drawing.Point(197, 201);
            this.btnButton3.Name = "btnButton3";
            this.btnButton3.Size = new System.Drawing.Size(75, 23);
            this.btnButton3.TabIndex = 2;
            this.btnButton3.Text = "button3";
            this.btnButton3.UseVisualStyleBackColor = true;
            this.btnButton3.Click += new System.EventHandler(this.btnButton3_Click);
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(12, 256);
            this.trackBar1.Maximum = 200;
            this.trackBar1.Minimum = 1;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(435, 45);
            this.trackBar1.TabIndex = 3;
            this.trackBar1.Value = 1;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(321, 111);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(309, 55);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 22);
            this.textBox1.TabIndex = 5;
            this.textBox1.Text = "3";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(333, 27);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 6;
            this.button2.Text = "Com";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // trackBar2
            // 
            this.trackBar2.Location = new System.Drawing.Point(12, 317);
            this.trackBar2.Maximum = 100;
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(435, 45);
            this.trackBar2.TabIndex = 3;
            this.trackBar2.ValueChanged += new System.EventHandler(this.trackBar2_ValueChanged);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(298, 167);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 7;
            this.button3.Text = "button3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(26, 365);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "label1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 388);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "label1";
            // 
            // timerAutoSetTime
            // 
            this.timerAutoSetTime.Enabled = true;
            this.timerAutoSetTime.Interval = 3600000;
            this.timerAutoSetTime.Tick += new System.EventHandler(this.timerAutoSetTime_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(476, 421);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.trackBar2);
            this.Controls.Add(this.trackBar1);
            this.Controls.Add(this.btnButton3);
            this.Controls.Add(this.btnButton2);
            this.Controls.Add(this.btnButton1);
            this.Controls.Add(this.btnTemperature);
            this.Controls.Add(this.btnMarquee);
            this.Controls.Add(this.btnWorldLine);
            this.Controls.Add(this.btnStopwatch);
            this.Controls.Add(this.btnDate);
            this.Controls.Add(this.btnTime);
            this.Controls.Add(this.btnAutoSetTime);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnAutoSetTime;
        private System.Windows.Forms.Button btnTime;
        private System.Windows.Forms.Button btnDate;
        private System.Windows.Forms.Button btnStopwatch;
        private System.Windows.Forms.Button btnWorldLine;
        private System.Windows.Forms.Button btnMarquee;
        private System.Windows.Forms.Button btnTemperature;
        private System.Windows.Forms.Button btnButton1;
        private System.Windows.Forms.Button btnButton2;
        private System.Windows.Forms.Button btnButton3;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TrackBar trackBar2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Timer timerAutoSetTime;
    }
}

