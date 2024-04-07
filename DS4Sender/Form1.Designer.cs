namespace DS4Sender
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            label1 = new Label();
            startButton = new Button();
            controllerCheck = new Button();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.BackColor = Color.FromArgb(128, 255, 128);
            label1.Font = new Font("Microsoft Sans Serif", 26F, FontStyle.Regular, GraphicsUnit.Point);
            label1.Location = new Point(164, 124);
            label1.Name = "label1";
            label1.Size = new Size(468, 52);
            label1.TabIndex = 0;
            label1.Text = "Controller is connected";
           // label1.Click += label1_Click;
            // 
            // startButton
            // 
            startButton.AutoSize = true;
            startButton.Font = new Font("Segoe UI", 25F, FontStyle.Regular, GraphicsUnit.Point);
            startButton.Location = new Point(54, 227);
            startButton.Name = "startButton";
            startButton.Size = new Size(708, 67);
            startButton.TabIndex = 1;
            startButton.Text = "Start Joystick Tracking";
            startButton.UseVisualStyleBackColor = true;
            startButton.Click += startButton_Click;
            // 
            // controllerCheck
            // 
            controllerCheck.AutoSize = true;
            controllerCheck.Font = new Font("Segoe UI", 15F, FontStyle.Regular, GraphicsUnit.Point);
            controllerCheck.Location = new Point(268, 335);
            controllerCheck.Name = "controllerCheck";
            controllerCheck.Size = new Size(247, 45);
            controllerCheck.TabIndex = 2;
            controllerCheck.Text = "Check for Controller";
            controllerCheck.UseVisualStyleBackColor = true;
            controllerCheck.Click += controllerCheck_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(controllerCheck);
            Controls.Add(startButton);
            Controls.Add(label1);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private Button startButton;
        private Button controllerCheck;
    }
}