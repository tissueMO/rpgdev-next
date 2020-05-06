namespace Editor.Dialog.Database {
	partial class dlgEditCurve {
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.label1 = new System.Windows.Forms.Label();
            this.numBase = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.cmbTargetDB = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.numIndex = new System.Windows.Forms.NumericUpDown();
            this.numCoefficient = new System.Windows.Forms.NumericUpDown();
            this.crtPreview = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.ltvTable = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            ((System.ComponentModel.ISupportInitialize)(this.numBase)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIndex)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numCoefficient)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.crtPreview)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(437, 107);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 15);
            this.label1.TabIndex = 3;
            this.label1.Text = "基底値:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // numBase
            // 
            this.numBase.Location = new System.Drawing.Point(519, 103);
            this.numBase.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numBase.Name = "numBase";
            this.numBase.Size = new System.Drawing.Size(118, 23);
            this.numBase.TabIndex = 4;
            this.numBase.ValueChanged += new System.EventHandler(this.numBase_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(437, 74);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 15);
            this.label4.TabIndex = 1;
            this.label4.Text = "対象項目:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cmbTargetDB
            // 
            this.cmbTargetDB.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbTargetDB.Enabled = false;
            this.cmbTargetDB.FormattingEnabled = true;
            this.cmbTargetDB.Location = new System.Drawing.Point(519, 70);
            this.cmbTargetDB.Name = "cmbTargetDB";
            this.cmbTargetDB.Size = new System.Drawing.Size(117, 23);
            this.cmbTargetDB.TabIndex = 2;
            this.cmbTargetDB.SelectedIndexChanged += new System.EventHandler(this.cmbTargetDB_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(437, 173);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(74, 15);
            this.label2.TabIndex = 7;
            this.label2.Text = "変化ミリ指数:";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(437, 140);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 15);
            this.label3.TabIndex = 5;
            this.label3.Text = "変化ミリ係数:";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // numIndex
            // 
            this.numIndex.Location = new System.Drawing.Point(519, 169);
            this.numIndex.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numIndex.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numIndex.Name = "numIndex";
            this.numIndex.Size = new System.Drawing.Size(118, 23);
            this.numIndex.TabIndex = 8;
            this.numIndex.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numIndex.ValueChanged += new System.EventHandler(this.numIndex_ValueChanged);
            // 
            // numCoefficient
            // 
            this.numCoefficient.Location = new System.Drawing.Point(519, 136);
            this.numCoefficient.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numCoefficient.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numCoefficient.Name = "numCoefficient";
            this.numCoefficient.Size = new System.Drawing.Size(118, 23);
            this.numCoefficient.TabIndex = 6;
            this.numCoefficient.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numCoefficient.ValueChanged += new System.EventHandler(this.numCoefficient_ValueChanged);
            // 
            // crtPreview
            // 
            this.crtPreview.BorderlineColor = System.Drawing.Color.Black;
            this.crtPreview.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
            this.crtPreview.BorderSkin.PageColor = System.Drawing.Color.Transparent;
            this.crtPreview.BorderSkin.SkinStyle = System.Windows.Forms.DataVisualization.Charting.BorderSkinStyle.Emboss;
            chartArea1.AxisX.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.True;
            chartArea1.AxisX.IsLabelAutoFit = false;
            chartArea1.AxisX.LabelStyle.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            chartArea1.AxisX.LabelStyle.Format = "Lv#";
            chartArea1.AxisX.MajorGrid.Interval = 0D;
            chartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.Gray;
            chartArea1.AxisX.MajorTickMark.LineColor = System.Drawing.Color.Gray;
            chartArea1.AxisX.Maximum = 100D;
            chartArea1.AxisX.Minimum = 1D;
            chartArea1.AxisX.MinorGrid.Enabled = true;
            chartArea1.AxisX.MinorGrid.LineColor = System.Drawing.Color.Gainsboro;
            chartArea1.AxisX.MinorTickMark.Enabled = true;
            chartArea1.AxisX.MinorTickMark.LineColor = System.Drawing.Color.Gainsboro;
            chartArea1.AxisX.TitleAlignment = System.Drawing.StringAlignment.Far;
            chartArea1.AxisX.TitleFont = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            chartArea1.AxisY.IsLabelAutoFit = false;
            chartArea1.AxisY.LabelStyle.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            chartArea1.AxisY.TitleFont = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            chartArea1.Name = "ChartArea1";
            this.crtPreview.ChartAreas.Add(chartArea1);
            this.crtPreview.Location = new System.Drawing.Point(12, 12);
            this.crtPreview.Name = "crtPreview";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.SplineArea;
            series1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            series1.YValuesPerPoint = 2;
            this.crtPreview.Series.Add(series1);
            this.crtPreview.Size = new System.Drawing.Size(409, 247);
            this.crtPreview.SuppressExceptions = true;
            this.crtPreview.TabIndex = 0;
            // 
            // ltvTable
            // 
            this.ltvTable.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.ltvTable.FullRowSelect = true;
            this.ltvTable.GridLines = true;
            this.ltvTable.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.ltvTable.Location = new System.Drawing.Point(12, 265);
            this.ltvTable.Name = "ltvTable";
            this.ltvTable.Size = new System.Drawing.Size(632, 175);
            this.ltvTable.TabIndex = 10;
            this.ltvTable.UseCompatibleStateImageBehavior = false;
            this.ltvTable.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Lv";
            this.columnHeader1.Width = 50;
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(436, 447);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
            this.ctlOKCancel1.TabIndex = 9;
            // 
            // dlgEditCurve
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(656, 488);
            this.Controls.Add(this.ltvTable);
            this.Controls.Add(this.numIndex);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.numBase);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numCoefficient);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.crtPreview);
            this.Controls.Add(this.cmbTargetDB);
            this.Controls.Add(this.label4);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgEditCurve";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "曲線生成";
            ((System.ComponentModel.ISupportInitialize)(this.numBase)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIndex)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numCoefficient)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.crtPreview)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.NumericUpDown numBase;
		private System.Windows.Forms.NumericUpDown numIndex;
		private System.Windows.Forms.NumericUpDown numCoefficient;
		private System.Windows.Forms.DataVisualization.Charting.Chart crtPreview;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ComboBox cmbTargetDB;
		private System.Windows.Forms.ListView ltvTable;
		private System.Windows.Forms.ColumnHeader columnHeader1;
	}
}