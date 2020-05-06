namespace Editor.Dialog.Map {
	partial class dlgMapGenerator {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.label1 = new System.Windows.Forms.Label();
			this.panel1 = new System.Windows.Forms.Panel();
			this.lblPreview = new System.Windows.Forms.Label();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnDoGenerate = new System.Windows.Forms.Button();
			this.numComplex = new System.Windows.Forms.NumericUpDown();
			this.label7 = new System.Windows.Forms.Label();
			this.cmbDestLayer = new System.Windows.Forms.ComboBox();
			this.label6 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.uctlTileCeil = new Editor.CtrlComponent.Map.ctlEVGraphics();
			this.uctlTileWall = new Editor.CtrlComponent.Map.ctlEVGraphics();
			this.uctlTileFloor = new Editor.CtrlComponent.Map.ctlEVGraphics();
			this.panel1.SuspendLayout();
			this.groupBox1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numComplex)).BeginInit();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(8, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(80, 15);
			this.label1.TabIndex = 0;
			this.label1.Text = "簡易プレビュー:";
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.AutoScroll = true;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel1.Controls.Add(this.lblPreview);
			this.panel1.Font = new System.Drawing.Font("ＭＳ ゴシック", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.panel1.Location = new System.Drawing.Point(11, 27);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(534, 362);
			this.panel1.TabIndex = 1;
			// 
			// lblPreview
			// 
			this.lblPreview.AutoSize = true;
			this.lblPreview.Location = new System.Drawing.Point(0, 0);
			this.lblPreview.Name = "lblPreview";
			this.lblPreview.Size = new System.Drawing.Size(0, 12);
			this.lblPreview.TabIndex = 0;
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.groupBox1.Controls.Add(this.btnDoGenerate);
			this.groupBox1.Controls.Add(this.numComplex);
			this.groupBox1.Controls.Add(this.label7);
			this.groupBox1.Controls.Add(this.cmbDestLayer);
			this.groupBox1.Controls.Add(this.label6);
			this.groupBox1.Controls.Add(this.uctlTileCeil);
			this.groupBox1.Controls.Add(this.uctlTileWall);
			this.groupBox1.Controls.Add(this.uctlTileFloor);
			this.groupBox1.Controls.Add(this.label5);
			this.groupBox1.Controls.Add(this.label4);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.ForeColor = System.Drawing.Color.Blue;
			this.groupBox1.Location = new System.Drawing.Point(551, 27);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(207, 368);
			this.groupBox1.TabIndex = 2;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "設定項目";
			// 
			// btnDoGenerate
			// 
			this.btnDoGenerate.ForeColor = System.Drawing.Color.Black;
			this.btnDoGenerate.Location = new System.Drawing.Point(42, 327);
			this.btnDoGenerate.Name = "btnDoGenerate";
			this.btnDoGenerate.Size = new System.Drawing.Size(132, 28);
			this.btnDoGenerate.TabIndex = 10;
			this.btnDoGenerate.Text = "ランダム生成実行";
			this.btnDoGenerate.UseVisualStyleBackColor = true;
			this.btnDoGenerate.Click += new System.EventHandler(this.btnDoGenerate_Click);
			// 
			// numComplex
			// 
			this.numComplex.ForeColor = System.Drawing.Color.Black;
			this.numComplex.Location = new System.Drawing.Point(109, 290);
			this.numComplex.Maximum = new decimal(new int[] {
            5,
            0,
            0,
            0});
			this.numComplex.Name = "numComplex";
			this.numComplex.Size = new System.Drawing.Size(87, 23);
			this.numComplex.TabIndex = 9;
			this.numComplex.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.ForeColor = System.Drawing.Color.Black;
			this.label7.Location = new System.Drawing.Point(10, 294);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(48, 15);
			this.label7.TabIndex = 8;
			this.label7.Text = "複雑度:";
			// 
			// cmbDestLayer
			// 
			this.cmbDestLayer.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbDestLayer.ForeColor = System.Drawing.Color.Black;
			this.cmbDestLayer.FormattingEnabled = true;
			this.cmbDestLayer.Location = new System.Drawing.Point(109, 258);
			this.cmbDestLayer.Name = "cmbDestLayer";
			this.cmbDestLayer.Size = new System.Drawing.Size(87, 23);
			this.cmbDestLayer.TabIndex = 7;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.ForeColor = System.Drawing.Color.Black;
			this.label6.Location = new System.Drawing.Point(10, 261);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(86, 15);
			this.label6.TabIndex = 6;
			this.label6.Text = "描画先レイヤー:";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.ForeColor = System.Drawing.Color.Black;
			this.label5.Location = new System.Drawing.Point(10, 182);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(89, 15);
			this.label5.TabIndex = 4;
			this.label5.Text = "天井タイル [■]:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.ForeColor = System.Drawing.Color.Black;
			this.label4.Location = new System.Drawing.Point(10, 103);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(77, 15);
			this.label4.TabIndex = 2;
			this.label4.Text = "壁タイル [壁]:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.ForeColor = System.Drawing.Color.Black;
			this.label3.Location = new System.Drawing.Point(10, 24);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(73, 15);
			this.label3.TabIndex = 0;
			this.label3.Text = "床タイル [　]:";
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label2.ForeColor = System.Drawing.SystemColors.GrayText;
			this.label2.Location = new System.Drawing.Point(12, 392);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(470, 50);
			this.label2.TabIndex = 4;
			this.label2.Text = "生成されるマップは機械的な角ばった通路や小部屋から構成されます。\r\nあくまでもダンジョンの原型を作る目的としてのみ利用して、細かい修正はご自身で行って下さい。";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ctlOKCancel1.DisenabledAcceptButton = false;
			this.ctlOKCancel1.DisenabledCancelButton = false;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(549, 408);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.OKTitle = "OK";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 3;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
			// 
			// uctlTileCeil
			// 
			this.uctlTileCeil.EnabledTileset = true;
			this.uctlTileCeil.FileName = "";
			this.uctlTileCeil.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlTileCeil.ForeColor = System.Drawing.Color.Black;
			this.uctlTileCeil.Location = new System.Drawing.Point(109, 182);
			this.uctlTileCeil.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlTileCeil.Name = "uctlTileCeil";
			this.uctlTileCeil.OnlyTileset = true;
			this.uctlTileCeil.Size = new System.Drawing.Size(64, 64);
			this.uctlTileCeil.TabIndex = 5;
			// 
			// uctlTileWall
			// 
			this.uctlTileWall.EnabledTileset = true;
			this.uctlTileWall.FileName = "";
			this.uctlTileWall.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlTileWall.ForeColor = System.Drawing.Color.Black;
			this.uctlTileWall.Location = new System.Drawing.Point(109, 103);
			this.uctlTileWall.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlTileWall.Name = "uctlTileWall";
			this.uctlTileWall.OnlyTileset = true;
			this.uctlTileWall.Size = new System.Drawing.Size(64, 64);
			this.uctlTileWall.TabIndex = 3;
			// 
			// uctlTileFloor
			// 
			this.uctlTileFloor.EnabledTileset = true;
			this.uctlTileFloor.FileName = "";
			this.uctlTileFloor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlTileFloor.ForeColor = System.Drawing.Color.Black;
			this.uctlTileFloor.Location = new System.Drawing.Point(109, 24);
			this.uctlTileFloor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlTileFloor.Name = "uctlTileFloor";
			this.uctlTileFloor.OnlyTileset = true;
			this.uctlTileFloor.Size = new System.Drawing.Size(64, 64);
			this.uctlTileFloor.TabIndex = 1;
			// 
			// dlgMapGenerator
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(766, 447);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(782, 486);
			this.Name = "dlgMapGenerator";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "ダンジョン自動生成";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numComplex)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Label lblPreview;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button btnDoGenerate;
		private System.Windows.Forms.NumericUpDown numComplex;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.ComboBox cmbDestLayer;
		private System.Windows.Forms.Label label6;
		private CtrlComponent.Map.ctlEVGraphics uctlTileCeil;
		private CtrlComponent.Map.ctlEVGraphics uctlTileWall;
		private CtrlComponent.Map.ctlEVGraphics uctlTileFloor;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.Label label2;
	}
}