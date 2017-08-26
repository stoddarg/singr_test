#pragma once

#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>					//for marshalling
#include <msclr/marshal_cppstd.h>	//for marshalling
#include "captureWaveformsPopup.h"
#include "changeAxesPopUp.h"
#include "setEnergyCutsPopUp.h"

#define BINARY_DATABUFFER_SIZE	49152
#define DATABUFFER_SIZE			12288

struct EventData
{
	double s_time;
	long long s_totalEvents;
	int s_eventCount;
	double s_baseline;
	double s_shortInt;
	double s_longInt;
	double s_fullInt;
	double s_baselineAvg;
	double s_PSDvalue;
	double s_energy;
};

namespace lunaHMaptestbed {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			findPorts();
			psdCap_run = false;
			backgroundWorker2->RunWorkerAsync();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}


	protected:













	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::ComboBox^  comboBox1;












	private: System::Windows::Forms::Label^  label6;


	private: bool psdCap_run;
	private: array<int>^ g_dataBuffer;
	private: array<unsigned char>^ g_binaryDataBuffer;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  restartToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  diagnosticsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  captureWaveformsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  optionsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  option1ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  option2ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cOMPortToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  closeCOMToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  restartCOMToolStripMenuItem;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  ch_PSD;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  ch_Spectrum;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  ch_FOM;
	private: System::Windows::Forms::Button^  b_SetIntegrationTimes;

	private: System::Windows::Forms::TextBox^  tb_baseline;
	private: System::Windows::Forms::TextBox^  tb_short;
	private: System::Windows::Forms::TextBox^  tb_long;
	private: System::Windows::Forms::TextBox^  tb_full;




	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  tb_trigger;
	private: System::Windows::Forms::Button^  b_SetThreshold;
	private: System::Windows::Forms::TextBox^  tb_updates;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  b_capturePSD;
	private: System::Windows::Forms::Button^  b_saveFile;
	private: System::Windows::Forms::CheckBox^  chk_stf;
	private: System::Windows::Forms::CheckBox^  chk_atf;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::TextBox^  tb_savefilename;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::ToolStripMenuItem^  pSDOptionsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  changeAxesPSDToolStripMenuItem;

private: System::Windows::Forms::ToolStripMenuItem^  clearChartPSDToolStripMenuItem;


	private: System::Windows::Forms::ToolStripMenuItem^  spectrumOptionsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  changeAxesSpectrumToolStripMenuItem1;
private: System::Windows::Forms::ToolStripMenuItem^  clearChartSpectrumToolStripMenuItem1;


	private: System::Windows::Forms::ToolStripMenuItem^  changeBinWidthToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  fOMOptionsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  changeAxesFOMToolStripMenuItem2;
private: System::Windows::Forms::ToolStripMenuItem^  clearChartFOMToolStripMenuItem2;
private: System::Windows::Forms::ToolStripMenuItem^  addEnergyCutsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  addFOMCutsToolStripMenuItem;
private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
private: System::Windows::Forms::TextBox^  tb_counterbox;

private: System::ComponentModel::BackgroundWorker^  backgroundWorker2;
private: System::Windows::Forms::Timer^  timer1;







	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Series^  series6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Series^  series7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->restartToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->diagnosticsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->captureWaveformsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cOMPortToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->closeCOMToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->restartCOMToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pSDOptionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeAxesPSDToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->clearChartPSDToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->spectrumOptionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeAxesSpectrumToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->clearChartSpectrumToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeBinWidthToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addEnergyCutsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fOMOptionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeAxesFOMToolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->clearChartFOMToolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addFOMCutsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ch_PSD = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->ch_Spectrum = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->ch_FOM = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->b_SetIntegrationTimes = (gcnew System::Windows::Forms::Button());
			this->tb_baseline = (gcnew System::Windows::Forms::TextBox());
			this->tb_short = (gcnew System::Windows::Forms::TextBox());
			this->tb_long = (gcnew System::Windows::Forms::TextBox());
			this->tb_full = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->tb_trigger = (gcnew System::Windows::Forms::TextBox());
			this->b_SetThreshold = (gcnew System::Windows::Forms::Button());
			this->tb_updates = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->b_capturePSD = (gcnew System::Windows::Forms::Button());
			this->b_saveFile = (gcnew System::Windows::Forms::Button());
			this->chk_stf = (gcnew System::Windows::Forms::CheckBox());
			this->chk_atf = (gcnew System::Windows::Forms::CheckBox());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tb_savefilename = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->tb_counterbox = (gcnew System::Windows::Forms::TextBox());
			this->backgroundWorker2 = (gcnew System::ComponentModel::BackgroundWorker());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ch_PSD))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ch_Spectrum))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ch_FOM))->BeginInit();
			this->SuspendLayout();
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 115200;
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(1075, 394);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(139, 21);
			this->comboBox1->TabIndex = 9;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(1016, 397);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 13);
			this->label6->TabIndex = 27;
			this->label6->Text = L"COM Port";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->fileToolStripMenuItem,
					this->diagnosticsToolStripMenuItem, this->cOMPortToolStripMenuItem, this->pSDOptionsToolStripMenuItem, this->spectrumOptionsToolStripMenuItem,
					this->fOMOptionsToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1223, 24);
			this->menuStrip1->TabIndex = 28;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->restartToolStripMenuItem,
					this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// restartToolStripMenuItem
			// 
			this->restartToolStripMenuItem->Name = L"restartToolStripMenuItem";
			this->restartToolStripMenuItem->Size = System::Drawing::Size(110, 22);
			this->restartToolStripMenuItem->Text = L"Restart";
			this->restartToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::restartToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(110, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
			// 
			// diagnosticsToolStripMenuItem
			// 
			this->diagnosticsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->captureWaveformsToolStripMenuItem });
			this->diagnosticsToolStripMenuItem->Name = L"diagnosticsToolStripMenuItem";
			this->diagnosticsToolStripMenuItem->Size = System::Drawing::Size(80, 20);
			this->diagnosticsToolStripMenuItem->Text = L"Diagnostics";
			// 
			// captureWaveformsToolStripMenuItem
			// 
			this->captureWaveformsToolStripMenuItem->Name = L"captureWaveformsToolStripMenuItem";
			this->captureWaveformsToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->captureWaveformsToolStripMenuItem->Text = L"Capture Waveforms";
			this->captureWaveformsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::captureWaveformsToolStripMenuItem_Click);
			// 
			// cOMPortToolStripMenuItem
			// 
			this->cOMPortToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->closeCOMToolStripMenuItem,
					this->restartCOMToolStripMenuItem
			});
			this->cOMPortToolStripMenuItem->Name = L"cOMPortToolStripMenuItem";
			this->cOMPortToolStripMenuItem->Size = System::Drawing::Size(72, 20);
			this->cOMPortToolStripMenuItem->Text = L"COM Port";
			// 
			// closeCOMToolStripMenuItem
			// 
			this->closeCOMToolStripMenuItem->Name = L"closeCOMToolStripMenuItem";
			this->closeCOMToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->closeCOMToolStripMenuItem->Text = L"Close COM";
			this->closeCOMToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::closeCOMToolStripMenuItem_Click);
			// 
			// restartCOMToolStripMenuItem
			// 
			this->restartCOMToolStripMenuItem->Name = L"restartCOMToolStripMenuItem";
			this->restartCOMToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->restartCOMToolStripMenuItem->Text = L"Restart COM";
			this->restartCOMToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::restartCOMToolStripMenuItem_Click);
			// 
			// pSDOptionsToolStripMenuItem
			// 
			this->pSDOptionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->changeAxesPSDToolStripMenuItem,
					this->clearChartPSDToolStripMenuItem
			});
			this->pSDOptionsToolStripMenuItem->Name = L"pSDOptionsToolStripMenuItem";
			this->pSDOptionsToolStripMenuItem->Size = System::Drawing::Size(85, 20);
			this->pSDOptionsToolStripMenuItem->Text = L"PSD Options";
			// 
			// changeAxesPSDToolStripMenuItem
			// 
			this->changeAxesPSDToolStripMenuItem->Name = L"changeAxesPSDToolStripMenuItem";
			this->changeAxesPSDToolStripMenuItem->Size = System::Drawing::Size(142, 22);
			this->changeAxesPSDToolStripMenuItem->Text = L"Change Axes";
			this->changeAxesPSDToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::changeAxesPSDToolStripMenuItem_Click);
			// 
			// clearChartPSDToolStripMenuItem
			// 
			this->clearChartPSDToolStripMenuItem->Name = L"clearChartPSDToolStripMenuItem";
			this->clearChartPSDToolStripMenuItem->Size = System::Drawing::Size(142, 22);
			this->clearChartPSDToolStripMenuItem->Text = L"Clear Chart";
			this->clearChartPSDToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::clearChartPSDToolStripMenuItem_Click);
			// 
			// spectrumOptionsToolStripMenuItem
			// 
			this->spectrumOptionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->changeAxesSpectrumToolStripMenuItem1,
					this->clearChartSpectrumToolStripMenuItem1, this->changeBinWidthToolStripMenuItem, this->addEnergyCutsToolStripMenuItem
			});
			this->spectrumOptionsToolStripMenuItem->Name = L"spectrumOptionsToolStripMenuItem";
			this->spectrumOptionsToolStripMenuItem->Size = System::Drawing::Size(115, 20);
			this->spectrumOptionsToolStripMenuItem->Text = L"Spectrum Options";
			// 
			// changeAxesSpectrumToolStripMenuItem1
			// 
			this->changeAxesSpectrumToolStripMenuItem1->Name = L"changeAxesSpectrumToolStripMenuItem1";
			this->changeAxesSpectrumToolStripMenuItem1->Size = System::Drawing::Size(173, 22);
			this->changeAxesSpectrumToolStripMenuItem1->Text = L"Change Axes";
			this->changeAxesSpectrumToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::changeAxesSpectrumToolStripMenuItem1_Click);
			// 
			// clearChartSpectrumToolStripMenuItem1
			// 
			this->clearChartSpectrumToolStripMenuItem1->Name = L"clearChartSpectrumToolStripMenuItem1";
			this->clearChartSpectrumToolStripMenuItem1->Size = System::Drawing::Size(173, 22);
			this->clearChartSpectrumToolStripMenuItem1->Text = L"Clear Chart";
			this->clearChartSpectrumToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::clearChartSpectrumToolStripMenuItem1_Click);
			// 
			// changeBinWidthToolStripMenuItem
			// 
			this->changeBinWidthToolStripMenuItem->Name = L"changeBinWidthToolStripMenuItem";
			this->changeBinWidthToolStripMenuItem->Size = System::Drawing::Size(173, 22);
			this->changeBinWidthToolStripMenuItem->Text = L"Change Bin Width";
			// 
			// addEnergyCutsToolStripMenuItem
			// 
			this->addEnergyCutsToolStripMenuItem->Name = L"addEnergyCutsToolStripMenuItem";
			this->addEnergyCutsToolStripMenuItem->Size = System::Drawing::Size(173, 22);
			this->addEnergyCutsToolStripMenuItem->Text = L"Add Cuts on PSD...";
			this->addEnergyCutsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::addEnergyCutsToolStripMenuItem_Click);
			// 
			// fOMOptionsToolStripMenuItem
			// 
			this->fOMOptionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->changeAxesFOMToolStripMenuItem2,
					this->clearChartFOMToolStripMenuItem2, this->addFOMCutsToolStripMenuItem
			});
			this->fOMOptionsToolStripMenuItem->Name = L"fOMOptionsToolStripMenuItem";
			this->fOMOptionsToolStripMenuItem->Size = System::Drawing::Size(90, 20);
			this->fOMOptionsToolStripMenuItem->Text = L"FOM Options";
			// 
			// changeAxesFOMToolStripMenuItem2
			// 
			this->changeAxesFOMToolStripMenuItem2->Name = L"changeAxesFOMToolStripMenuItem2";
			this->changeAxesFOMToolStripMenuItem2->Size = System::Drawing::Size(188, 22);
			this->changeAxesFOMToolStripMenuItem2->Text = L"Change Axes";
			this->changeAxesFOMToolStripMenuItem2->Click += gcnew System::EventHandler(this, &MyForm::changeAxesFOMToolStripMenuItem2_Click);
			// 
			// clearChartFOMToolStripMenuItem2
			// 
			this->clearChartFOMToolStripMenuItem2->Name = L"clearChartFOMToolStripMenuItem2";
			this->clearChartFOMToolStripMenuItem2->Size = System::Drawing::Size(188, 22);
			this->clearChartFOMToolStripMenuItem2->Text = L"Clear Chart";
			this->clearChartFOMToolStripMenuItem2->Click += gcnew System::EventHandler(this, &MyForm::clearChartFOMToolStripMenuItem2_Click);
			// 
			// addFOMCutsToolStripMenuItem
			// 
			this->addFOMCutsToolStripMenuItem->Name = L"addFOMCutsToolStripMenuItem";
			this->addFOMCutsToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->addFOMCutsToolStripMenuItem->Text = L"Add Cuts on Energy...";
			this->addFOMCutsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::addFOMCutsToolStripMenuItem_Click);
			// 
			// ch_PSD
			// 
			this->ch_PSD->AntiAliasing = System::Windows::Forms::DataVisualization::Charting::AntiAliasingStyles::None;
			chartArea1->AxisX->IsStartedFromZero = false;
			chartArea1->AxisX->Maximum = 200000;
			chartArea1->AxisX->Minimum = 0;
			chartArea1->AxisY->Maximum = 2;
			chartArea1->AxisY->Minimum = 0;
			chartArea1->Name = L"ChartArea1";
			this->ch_PSD->ChartAreas->Add(chartArea1);
			this->ch_PSD->Location = System::Drawing::Point(13, 28);
			this->ch_PSD->Name = L"ch_PSD";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::FastPoint;
			series1->MarkerSize = 2;
			series1->Name = L"Series1";
			series2->BorderWidth = 4;
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series2->Color = System::Drawing::Color::Blue;
			series2->Name = L"series_ECutLower";
			series3->BorderWidth = 4;
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series3->Color = System::Drawing::Color::Blue;
			series3->Name = L"series_ECutUpper";
			series4->BorderWidth = 4;
			series4->ChartArea = L"ChartArea1";
			series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series4->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			series4->Name = L"series_FOMCutLeft";
			series4->YValuesPerPoint = 2;
			series5->BorderWidth = 4;
			series5->ChartArea = L"ChartArea1";
			series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series5->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			series5->MarkerSize = 100;
			series5->Name = L"series_FOMCutRight";
			series5->YValuesPerPoint = 2;
			this->ch_PSD->Series->Add(series1);
			this->ch_PSD->Series->Add(series2);
			this->ch_PSD->Series->Add(series3);
			this->ch_PSD->Series->Add(series4);
			this->ch_PSD->Series->Add(series5);
			this->ch_PSD->Size = System::Drawing::Size(706, 343);
			this->ch_PSD->TabIndex = 29;
			this->ch_PSD->Text = L"chart1";
			this->ch_PSD->TextAntiAliasingQuality = System::Windows::Forms::DataVisualization::Charting::TextAntiAliasingQuality::Normal;
			// 
			// ch_Spectrum
			// 
			this->ch_Spectrum->AntiAliasing = System::Windows::Forms::DataVisualization::Charting::AntiAliasingStyles::None;
			chartArea2->AxisX->IsStartedFromZero = false;
			chartArea2->AxisX->Maximum = 200000;
			chartArea2->AxisX->Minimum = 0;
			chartArea2->AxisY->Maximum = 500;
			chartArea2->AxisY->Minimum = 0;
			chartArea2->Name = L"ChartArea1";
			this->ch_Spectrum->ChartAreas->Add(chartArea2);
			this->ch_Spectrum->IsSoftShadows = false;
			this->ch_Spectrum->Location = System::Drawing::Point(13, 377);
			this->ch_Spectrum->Name = L"ch_Spectrum";
			series6->ChartArea = L"ChartArea1";
			series6->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::StepLine;
			series6->MarkerSize = 2;
			series6->Name = L"Series1";
			this->ch_Spectrum->Series->Add(series6);
			this->ch_Spectrum->Size = System::Drawing::Size(706, 286);
			this->ch_Spectrum->TabIndex = 30;
			this->ch_Spectrum->Text = L"chart1";
			this->ch_Spectrum->TextAntiAliasingQuality = System::Windows::Forms::DataVisualization::Charting::TextAntiAliasingQuality::Normal;
			// 
			// ch_FOM
			// 
			chartArea3->AxisX->Maximum = 2;
			chartArea3->AxisX->Minimum = 0;
			chartArea3->AxisY->IsStartedFromZero = false;
			chartArea3->AxisY->Maximum = 500;
			chartArea3->AxisY->Minimum = 0;
			chartArea3->Name = L"ChartArea1";
			this->ch_FOM->ChartAreas->Add(chartArea3);
			this->ch_FOM->Location = System::Drawing::Point(726, 28);
			this->ch_FOM->Name = L"ch_FOM";
			series7->ChartArea = L"ChartArea1";
			series7->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Bar;
			series7->CustomProperties = L"PointWidth=0.01";
			series7->Name = L"Series1";
			this->ch_FOM->Series->Add(series7);
			this->ch_FOM->Size = System::Drawing::Size(488, 343);
			this->ch_FOM->TabIndex = 31;
			this->ch_FOM->Text = L"chart1";
			// 
			// b_SetIntegrationTimes
			// 
			this->b_SetIntegrationTimes->Location = System::Drawing::Point(761, 488);
			this->b_SetIntegrationTimes->Name = L"b_SetIntegrationTimes";
			this->b_SetIntegrationTimes->Size = System::Drawing::Size(140, 41);
			this->b_SetIntegrationTimes->TabIndex = 32;
			this->b_SetIntegrationTimes->Text = L"Set Integration Times";
			this->b_SetIntegrationTimes->UseVisualStyleBackColor = true;
			this->b_SetIntegrationTimes->Click += gcnew System::EventHandler(this, &MyForm::b_SetIntegrationTimes_Click);
			// 
			// tb_baseline
			// 
			this->tb_baseline->Location = System::Drawing::Point(791, 381);
			this->tb_baseline->Name = L"tb_baseline";
			this->tb_baseline->Size = System::Drawing::Size(140, 20);
			this->tb_baseline->TabIndex = 33;
			// 
			// tb_short
			// 
			this->tb_short->Location = System::Drawing::Point(791, 408);
			this->tb_short->Name = L"tb_short";
			this->tb_short->Size = System::Drawing::Size(140, 20);
			this->tb_short->TabIndex = 34;
			// 
			// tb_long
			// 
			this->tb_long->Location = System::Drawing::Point(791, 435);
			this->tb_long->Name = L"tb_long";
			this->tb_long->Size = System::Drawing::Size(140, 20);
			this->tb_long->TabIndex = 35;
			// 
			// tb_full
			// 
			this->tb_full->Location = System::Drawing::Point(791, 462);
			this->tb_full->Name = L"tb_full";
			this->tb_full->Size = System::Drawing::Size(140, 20);
			this->tb_full->TabIndex = 36;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(730, 387);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 13);
			this->label1->TabIndex = 37;
			this->label1->Text = L"Baseline";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(730, 411);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(47, 13);
			this->label2->TabIndex = 38;
			this->label2->Text = L"Short Int";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(730, 438);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(46, 13);
			this->label3->TabIndex = 39;
			this->label3->Text = L"Long Int";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(730, 465);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(38, 13);
			this->label4->TabIndex = 40;
			this->label4->Text = L"Full Int";
			// 
			// label5
			// 
			this->label5->Location = System::Drawing::Point(729, 539);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(63, 31);
			this->label5->TabIndex = 41;
			this->label5->Text = L"Trigger Threshold";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// tb_trigger
			// 
			this->tb_trigger->Location = System::Drawing::Point(791, 545);
			this->tb_trigger->Name = L"tb_trigger";
			this->tb_trigger->Size = System::Drawing::Size(140, 20);
			this->tb_trigger->TabIndex = 42;
			// 
			// b_SetThreshold
			// 
			this->b_SetThreshold->Location = System::Drawing::Point(761, 573);
			this->b_SetThreshold->Name = L"b_SetThreshold";
			this->b_SetThreshold->Size = System::Drawing::Size(140, 41);
			this->b_SetThreshold->TabIndex = 43;
			this->b_SetThreshold->Text = L"Set Trigger Threshold";
			this->b_SetThreshold->UseVisualStyleBackColor = true;
			this->b_SetThreshold->Click += gcnew System::EventHandler(this, &MyForm::b_SetThreshold_Click);
			// 
			// tb_updates
			// 
			this->tb_updates->Location = System::Drawing::Point(1016, 447);
			this->tb_updates->Name = L"tb_updates";
			this->tb_updates->ReadOnly = true;
			this->tb_updates->Size = System::Drawing::Size(198, 20);
			this->tb_updates->TabIndex = 44;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(960, 450);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(50, 13);
			this->label7->TabIndex = 45;
			this->label7->Text = L"Updates:";
			// 
			// b_capturePSD
			// 
			this->b_capturePSD->Location = System::Drawing::Point(1019, 473);
			this->b_capturePSD->Name = L"b_capturePSD";
			this->b_capturePSD->Size = System::Drawing::Size(140, 41);
			this->b_capturePSD->TabIndex = 46;
			this->b_capturePSD->Text = L"Capture PSD";
			this->b_capturePSD->UseVisualStyleBackColor = true;
			this->b_capturePSD->Click += gcnew System::EventHandler(this, &MyForm::b_capturePSD_Click);
			// 
			// b_saveFile
			// 
			this->b_saveFile->Location = System::Drawing::Point(974, 589);
			this->b_saveFile->Name = L"b_saveFile";
			this->b_saveFile->Size = System::Drawing::Size(107, 41);
			this->b_saveFile->TabIndex = 47;
			this->b_saveFile->Text = L"Choose Save File...";
			this->b_saveFile->UseVisualStyleBackColor = true;
			this->b_saveFile->Click += gcnew System::EventHandler(this, &MyForm::b_saveFile_Click);
			// 
			// chk_stf
			// 
			this->chk_stf->AutoSize = true;
			this->chk_stf->Location = System::Drawing::Point(1087, 591);
			this->chk_stf->Name = L"chk_stf";
			this->chk_stf->Size = System::Drawing::Size(107, 17);
			this->chk_stf->TabIndex = 48;
			this->chk_stf->Text = L"Save to New File";
			this->chk_stf->UseVisualStyleBackColor = true;
			// 
			// chk_atf
			// 
			this->chk_atf->AutoSize = true;
			this->chk_atf->Location = System::Drawing::Point(1087, 613);
			this->chk_atf->Name = L"chk_atf";
			this->chk_atf->Size = System::Drawing::Size(94, 17);
			this->chk_atf->TabIndex = 49;
			this->chk_atf->Text = L"Append to File";
			this->chk_atf->UseVisualStyleBackColor = true;
			// 
			// tb_savefilename
			// 
			this->tb_savefilename->Location = System::Drawing::Point(725, 642);
			this->tb_savefilename->Name = L"tb_savefilename";
			this->tb_savefilename->ReadOnly = true;
			this->tb_savefilename->Size = System::Drawing::Size(489, 20);
			this->tb_savefilename->TabIndex = 50;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(726, 624);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(98, 13);
			this->label8->TabIndex = 51;
			this->label8->Text = L"Save File Location:";
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker1_DoWork);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MyForm::backgroundWorker1_ProgressChanged);
			this->backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MyForm::backgroundWorker1_RunWorkerCompleted);
			// 
			// tb_counterbox
			// 
			this->tb_counterbox->Location = System::Drawing::Point(1174, 473);
			this->tb_counterbox->Name = L"tb_counterbox";
			this->tb_counterbox->ReadOnly = true;
			this->tb_counterbox->Size = System::Drawing::Size(20, 20);
			this->tb_counterbox->TabIndex = 52;
			// 
			// backgroundWorker2
			// 
			this->backgroundWorker2->WorkerReportsProgress = true;
			this->backgroundWorker2->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker2_DoWork);
			this->backgroundWorker2->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MyForm::backgroundWorker2_ProgressChanged);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1223, 673);
			this->Controls->Add(this->tb_counterbox);
			this->Controls->Add(this->chk_atf);
			this->Controls->Add(this->chk_stf);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->tb_savefilename);
			this->Controls->Add(this->b_saveFile);
			this->Controls->Add(this->b_capturePSD);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->tb_updates);
			this->Controls->Add(this->b_SetThreshold);
			this->Controls->Add(this->tb_trigger);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->tb_full);
			this->Controls->Add(this->tb_long);
			this->Controls->Add(this->tb_short);
			this->Controls->Add(this->tb_baseline);
			this->Controls->Add(this->b_SetIntegrationTimes);
			this->Controls->Add(this->ch_FOM);
			this->Controls->Add(this->ch_Spectrum);
			this->Controls->Add(this->ch_PSD);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Luna H Map GUI";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ch_PSD))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ch_Spectrum))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ch_FOM))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		
private:	//declare class-wide variables here
	double m_energyLowerCut{ 0.0 };
	double m_energyUpperCut{ 0.0 };
	double m_fomLeftCut{ 0.0 };
	double m_fomRightCut{ 0.0 };
		
private: void findPorts(void)
{
	array<Object^>^ objectArray = SerialPort::GetPortNames();
	this->comboBox1->Items->AddRange(objectArray);		
}

private: System::Void captureWaveformsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	/* When moving between screens, need to close open ports*/
	if (this->serialPort1->IsOpen)
	{
		this->serialPort1->Close();
	}
	
	/*show the capture waveforms pop up */
	captureWaveformsPopup ^ captureWFs = gcnew captureWaveformsPopup();
	captureWFs->Show();
}//eocapturewaveformsshow

private: System::Void b_SetIntegrationTimes_Click(System::Object^  sender, System::EventArgs^  e) {
	int baselineInt = 0;
	int shortInt = 0;
	int longInt = 0;
	int fullInt = 0;
	String^ baselineMessage = this->tb_baseline->Text;
	String^ shortMessage = this->tb_short->Text;
	String^ longMessage = this->tb_long->Text;
	String^ fullMessage = this->tb_full->Text;

	/* Open the serial port */
	if (this->comboBox1->Text == String::Empty) {
		this->tb_updates->Text = "Select a port above.";
		return;
	}
	else
	{
		if (!this->serialPort1->IsOpen) {
			this->serialPort1->PortName = this->comboBox1->Text;
			this->serialPort1->Open();
			this->b_SetIntegrationTimes->Enabled = false;
		}
		else {
			this->tb_updates->Text = (this->comboBox1->Text) + " is still open.";
			this->b_SetIntegrationTimes->Enabled = false;
			Application::DoEvents();
		}
	}

	this->serialPort1->WriteLine("4");	//change integrals at the main menu
	Sleep(500);							//retMessage = this->serialPort1->ReadLine();	//"enter each integral..."
	this->serialPort1->WriteLine(baselineMessage);
	Sleep(500);
	this->serialPort1->WriteLine(shortMessage);
	Sleep(500);
	this->serialPort1->WriteLine(longMessage);
	Sleep(500);
	this->serialPort1->WriteLine(fullMessage);
	Sleep(500);

	this->serialPort1->DiscardInBuffer();

	this->tb_updates->Text = "Integral times have been set.";
	this->b_SetIntegrationTimes->Enabled = true;
}//eoSetIntegrationTimes

private: System::Void b_SetThreshold_Click(System::Object^  sender, System::EventArgs^  e) {
	/* This button click will allow the user to change the trigger threshold of the system over the serial port */
	String^ retMessage = "";	//allows for testing
	String^ thresholdMessage = this->tb_trigger->Text;

	/* Open the serial port */
	if (this->comboBox1->Text == String::Empty) {
		this->tb_updates->Text = "Select a port above.";
		return;
	}
	else
	{
		if (!this->serialPort1->IsOpen) {
			this->serialPort1->PortName = this->comboBox1->Text;
			this->serialPort1->Open();
			this->b_SetThreshold->Enabled = false;
		}
		else {
			this->tb_updates->Text = (this->comboBox1->Text) + " is already open.";
			this->b_SetThreshold->Enabled = false;
			Application::DoEvents();
		}
	}

	/* Send commands to the uZ over the serial connection */
	this->serialPort1->WriteLine("3");				//choose change threshold from main menu
	Sleep(500);
	this->serialPort1->WriteLine(thresholdMessage);	//write the threshold specified by the user
	//retMessage = this->serialPort1->ReadLine();		//read the line that ask for the new threshold	//removed all feedback from devkit
	Application::DoEvents();
	Sleep(1200);

	this->serialPort1->DiscardInBuffer();

	///* Clean up */
	this->tb_updates->Text = "Trigger threshold has been set.";
	this->b_SetThreshold->Enabled = true;

	//this->serialPort1->WriteLine("a\r");
	//retMessage = this->serialPort1->ReadLine();
	//this->tb_updates->Text = retMessage;
	//Application::DoEvents();
	//this->b_SetThreshold->Enabled = true;

	//unsigned char one = 1;
	//unsigned char two = 2;
	//unsigned char three = 4;
	//unsigned char four = 8;

	//int myint{ 0 };
	//myint = (one << (8 * 3)) | (two << (8 * 2)) | (three << (8 * 1)) | four;
	//this->tb_updates->Text = myint + " ";

}//eoSetThreshold

private: System::Void b_capturePSD_Click(System::Object^  sender, System::EventArgs^  e) {

	////background worker stuff!
	//psdCap_run = !psdCap_run;		//init'd as false
	//if (psdCap_run)
	//{
	//	this->b_capturePSD->Text = "Stop Capture";
	//}
	//if (!psdCap_run)	//stop capturing data
	//{
	//	this->b_capturePSD->Text = "Capture PSD";
	//	this->tb_updates->Text = "Stop button pressed.";
	//	return;	//returns out of this function call to the still running call of this function
	//}
	//if (psdCap_run && (chk_atf->Checked || chk_stf->Checked))	//if we are running and one of the check buttons is checked, get the file name to pass
	//{
	//	if (this->tb_savefilename->Text == String::Empty)		//if checked, but no file name, return
	//	{
	//		this->tb_updates->Text = "Please choose a file save or append location.";
	//		this->b_capturePSD->Text = "Capture PSD";
	//		psdCap_run = !psdCap_run;
	//		return;
	//	}

	//	/* Get the filename */
	//	String^ s_fileName = this->saveFileDialog1->FileName;
	//}
	



	/* This button will send commands to open a serial connection and get processed data */
	psdCap_run = !psdCap_run;		//init'd as false
	if (psdCap_run)
	{
		this->b_capturePSD->Text = "Stop Capture";
	}
	if (!psdCap_run)	//stop capturing data
	{
		this->b_capturePSD->Text = "Capture PSD";
		this->tb_updates->Text = "Stop button pressed.";
		return;	//returns out of this function call to the still running call of this function
	}

	/* create an output file stream object */
	std::ofstream outputFile;

	if (psdCap_run && (chk_atf->Checked || chk_stf->Checked))	//if we are running and one of the check buttons is checked, open the save file
	{
		if (this->tb_savefilename->Text == String::Empty)
		{
			this->tb_updates->Text = "Please choose a file save or append location.";
			this->b_capturePSD->Text = "Capture PSD";
			psdCap_run = !psdCap_run;
			return;
		}

		/* Get the filename */
		std::string str_fileName;
		String^ s_fileName = this->saveFileDialog1->FileName;
		str_fileName = msclr::interop::marshal_as<std::string>(s_fileName);

		/* open the file */
		outputFile.open(str_fileName, std::ios::app);

		if (!outputFile)	//if we can't open the file
		{
			this->tb_updates->Text = "Could not access the specified file.";
			this->b_capturePSD->Text = "Capture PSD";
			psdCap_run = !psdCap_run;
			return;
		}
	}

	/* Declare variables */
	int iBaselineValue{ 0 };
	int iShortIntValue{ 0 };
	int iLongIntValue{ 0 };
	int iFullIntValue{ 0 };
	int num_bl_samples{ 0 };
	int num_si_samples{ 0 };
	int num_li_samples{ 0 };
	int num_fi_samples{ 0 };
	double bl1{ 0 };
	double bl2{ 0 };
	double bl3{ 0 };
	double bl4{ 0 };
	int ID{ 0 };
	double bl_avg{ 0.0 };
	double baseline_int{ 0.0 };
	double short_int{ 0.0 };
	double long_int{ 0.0 };
	double full_int{ 0.0 };
	double psd{ 0.0 };
	double energy{ 0.0 };
	EventData eventStruct{};

	double dPSDXmin{ 0 };
	double dPSDXmax{ 0 };
	int iSpectrumArray[1000]{};
	double dSpectrumDomain{ 0.0 };
	double dBinSize{ 0.0 };
	int iSpectrumArrayIndex{ 0 };
	double dSpectrumBin{ 0.0 };
	int missedEvents{ 0 };

	double dPSDYmin{ 0.0 };
	double dPSDYmax{ 0.0 };
	double dFOMRange{ 0.0 };
	double dFOMBinSize{ 0.0 };
	double dFOMBinNumber{ 0.0 };
	int iFOMBins{ 100 };
	int * iFOMArray = new int[iFOMBins]{};	//need to work on how to be able to change this (probably should use std::vector)
	int iFOMArrayIndex{ 0 };

	/* Determine the domain and set axes for the spectrum graph */
	dPSDXmin = this->ch_PSD->ChartAreas[0]->AxisX->Minimum;
	dPSDXmax = this->ch_PSD->ChartAreas[0]->AxisX->Maximum;
	this->ch_Spectrum->ChartAreas[0]->AxisX->Minimum = dPSDXmin;
	this->ch_Spectrum->ChartAreas[0]->AxisX->Maximum = dPSDXmax;
	dSpectrumDomain = dPSDXmax - dPSDXmin;
	dBinSize = dSpectrumDomain / 1000.0;

	/* Determine the range, set axes, and calculate the bin size for the FOM graph */
	dPSDYmin = this->ch_PSD->ChartAreas[0]->AxisY->Minimum;
	dPSDYmax = this->ch_PSD->ChartAreas[0]->AxisY->Maximum;
	this->ch_FOM->ChartAreas[0]->AxisX->Minimum = dPSDYmin;
	this->ch_FOM->ChartAreas[0]->AxisX->Maximum = dPSDYmax;
	dFOMRange = dPSDYmax - dPSDYmin;
	dFOMBinSize = dFOMRange / iFOMBins;

	/* Open the serial port */
	if (this->comboBox1->Text == String::Empty) 
	{
		this->tb_updates->Text = "Select a port above.";
		this->b_capturePSD->Text = "Capture PSD";
		psdCap_run = !psdCap_run;
		return;
	}
	else 
	{
		if (!this->serialPort1->IsOpen) {	//if the port isn't open, open it 
			this->serialPort1->PortName = this->comboBox1->Text;
			this->serialPort1->Open();			
		}
		else {	//if the port is open, just post that and continue using it
			this->tb_updates->Text = "Connected using port " + this->serialPort1->PortName;
		}
	}

	/* get the number of samples based on what is in the textboxes, or the default */
	/* if nothing is entered in the integration times textboxes, put the default values there */
	if (this->tb_baseline->Text == String::Empty)
	{
		num_bl_samples = ((200 - 8) / 4) + 1;		// default is -8, gives 49 samples
		num_si_samples = ((200 + 232) / 4) + 1;		// default is 232, gives 109
		num_li_samples = ((200 + 616) / 4) + 1;		// default is 616, gives 205
		num_fi_samples = ((200 + 8008) / 4) + 1;	// default is 8008, gives 2053

		this->tb_baseline->Text = (num_bl_samples * 4) - 52 + " ";	//fill in the empty textboxes with the following values
		this->tb_short->Text = (num_si_samples * 4) - 52 + " ";
		this->tb_long->Text = (num_li_samples * 4) - 52 + " ";
		this->tb_full->Text = (num_fi_samples * 4) - 52 + " ";
	}
	else	// if the user entered their own values, find the number of samples for those values
	{
		Int32::TryParse(this->tb_baseline->Text, iBaselineValue);
		Int32::TryParse(this->tb_short->Text, iShortIntValue);
		Int32::TryParse(this->tb_long->Text, iLongIntValue);
		Int32::TryParse(this->tb_full->Text, iFullIntValue);
		num_bl_samples = ((200 + iBaselineValue) / 4) + 1;
		num_si_samples = ((200 + iShortIntValue) / 4) + 1;
		num_li_samples = ((200 + iLongIntValue) / 4) + 1;
		num_fi_samples = ((200 + iFullIntValue) / 4) + 1;
	}

	/* Now we may send commands over the serial port */
	if (this->serialPort1->IsOpen)	//double check that the port is open
	{
		this->serialPort1->WriteLine("0");
		Sleep(500);
		this->serialPort1->WriteLine("4");
		Sleep(500);
		this->serialPort1->WriteLine("1");
		Sleep(500);

		this->serialPort1->WriteLine("2");
		this->tb_updates->Text = "Collecting...";
		//Sleep(5000);	//wait 5 seconds before sending a message asking for data from the uZ
	}
	else
	{
		this->tb_updates->Text = "Port is not open.";
		this->b_capturePSD->Text = "Capture PSD";
		psdCap_run = !psdCap_run;
		return;
	}
	
	/* Capture incoming data; process and plot it */
	int index{ 0 };
	int buffsize{ 0 };
	int numBuffers{ 0 };
	int bytesRead{ 0 };
	int offset{ 0 };
	int dataBuffer[100]{};
	String^ amessage = "";
	g_dataBuffer = gcnew array<int>(DATABUFFER_SIZE) {};
	g_binaryDataBuffer = gcnew array<unsigned char>(BINARY_DATABUFFER_SIZE) {};

/*	Int32::TryParse(this->serialPort1->ReadLine(), buffsize); */	//get the buffer size 
	while (psdCap_run)	//replace numBuffers with psdCap_run
	{
		Sleep(2000);	//ask for data every 10s 
		this->serialPort1->WriteLine("a\r");
		//amessage = this->serialPort1->ReadLine();	//wait for the uZ to sort the array//this will block until we receive the message

		while (index < DATABUFFER_SIZE)	//read the entire thing into our u8 buffer so we may sort it back into integers and save it
		{
			//bytesRead += this->serialPort1->Read(g_binaryDataBuffer, offset + bytesRead, this->serialPort1->BytesToRead);
			Int32::TryParse(this->serialPort1->ReadLine(), g_dataBuffer[index]);	//parse in the line that we are reading 
			outputFile << std::setw(11) << g_dataBuffer[index] << std::endl;
			index++;
		}
		index = 0;
		Application::DoEvents();
	}
	
	this->serialPort1->DiscardInBuffer();
	if (outputFile.is_open())
	{
		outputFile.close();
	}
	outputFile.close();
	delete[] iFOMArray;

	this->serialPort1->WriteLine("q\r");
	Sleep(500);

	this->tb_updates->Text = "Capture finished. Chart not erased.";
	this->tb_updates->Text = amessage;
	return;
}//eoCapturePSDclick

private: System::Void b_saveFile_Click(System::Object^  sender, System::EventArgs^  e) {
	/* create instance of a save file dialog */
	saveFileDialog1->ShowDialog();
	tb_savefilename->Text = saveFileDialog1->FileName;
}//eosavefileclick

private: System::Void changeAxesPSDToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	/* Open small pop-up which takes in values for axes and passes them back here */
	double xMin;
	double xMax;
	double yMin;
	double yMax;

	xMin = this->ch_PSD->ChartAreas[0]->AxisX->Minimum;
	xMax = this->ch_PSD->ChartAreas[0]->AxisX->Maximum;
	yMin = this->ch_PSD->ChartAreas[0]->AxisY->Minimum;
	yMax = this->ch_PSD->ChartAreas[0]->AxisY->Maximum;
	
	/* Show the change axes pop up */
	changeAxesPopUp ^ changePSDAxes = gcnew changeAxesPopUp( xMin, xMax, yMin, yMax);
	//Application::Run(changePSDAxes);
	changePSDAxes->ShowDialog();

	this->ch_PSD->ChartAreas[0]->AxisX->Minimum = changePSDAxes->getxMin();
	this->ch_PSD->ChartAreas[0]->AxisX->Maximum = changePSDAxes->getxMax();
	this->ch_PSD->ChartAreas[0]->AxisY->Minimum = changePSDAxes->getyMin();
	this->ch_PSD->ChartAreas[0]->AxisY->Maximum = changePSDAxes->getyMax();

	/* also change the spectrum chart because we need the two to be linked */
	this->ch_Spectrum->ChartAreas[0]->AxisX->Minimum = changePSDAxes->getxMin();
	this->ch_Spectrum->ChartAreas[0]->AxisX->Maximum = changePSDAxes->getxMax();
	this->ch_FOM->ChartAreas[0]->AxisY->Minimum = changePSDAxes->getyMin();
	this->ch_FOM->ChartAreas[0]->AxisY->Maximum = changePSDAxes->getyMax();

}//eochangepsdaxes

private: System::Void changeAxesSpectrumToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
	double xMin;
	double xMax;
	double yMin;
	double yMax;

	xMin = this->ch_Spectrum->ChartAreas[0]->AxisX->Minimum;
	xMax = this->ch_Spectrum->ChartAreas[0]->AxisX->Maximum;
	yMin = this->ch_Spectrum->ChartAreas[0]->AxisY->Minimum;
	yMax = this->ch_Spectrum->ChartAreas[0]->AxisY->Maximum;

	/* Show the change axes pop up */
	changeAxesPopUp ^ changeSpectrumAxes = gcnew changeAxesPopUp(xMin, xMax, yMin, yMax);
	//Application::Run(changePSDAxes);
	changeSpectrumAxes->ShowDialog();

	this->ch_Spectrum->ChartAreas[0]->AxisX->Minimum = changeSpectrumAxes->getxMin();
	this->ch_Spectrum->ChartAreas[0]->AxisX->Maximum = changeSpectrumAxes->getxMax();
	this->ch_Spectrum->ChartAreas[0]->AxisY->Minimum = changeSpectrumAxes->getyMin();
	this->ch_Spectrum->ChartAreas[0]->AxisY->Maximum = changeSpectrumAxes->getyMax();
}//eochangespectrumaxes

private: System::Void changeAxesFOMToolStripMenuItem2_Click(System::Object^  sender, System::EventArgs^  e) {
	double xMin;
	double xMax;
	double yMin;
	double yMax;

	xMin = this->ch_FOM->ChartAreas[0]->AxisX->Minimum;
	xMax = this->ch_FOM->ChartAreas[0]->AxisX->Maximum;
	yMin = this->ch_FOM->ChartAreas[0]->AxisY->Minimum;
	yMax = this->ch_FOM->ChartAreas[0]->AxisY->Maximum;

	/* Show the change axes pop up */
	changeAxesPopUp ^ changeFOMAxes = gcnew changeAxesPopUp(xMin, xMax, yMin, yMax);
	//Application::Run(changePSDAxes);
	changeFOMAxes->ShowDialog();

	this->ch_FOM->ChartAreas[0]->AxisX->Minimum = changeFOMAxes->getxMin();
	this->ch_FOM->ChartAreas[0]->AxisX->Maximum = changeFOMAxes->getxMax();
	this->ch_FOM->ChartAreas[0]->AxisY->Minimum = changeFOMAxes->getyMin();
	this->ch_FOM->ChartAreas[0]->AxisY->Maximum = changeFOMAxes->getyMax();
}//eochangefomaxes

private: System::Void clearChartPSDToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	this->ch_PSD->Series["Series1"]->Points->Clear();
	this->tb_updates->Text = "PSD Chart cleared.";
	Application::DoEvents();
}

private: System::Void clearChartSpectrumToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
	this->ch_Spectrum->Series["Series1"]->Points->Clear();
	this->tb_updates->Text = "Energy spectrum chart cleared.";
	Application::DoEvents();
}

private: System::Void clearChartFOMToolStripMenuItem2_Click(System::Object^  sender, System::EventArgs^  e) {
	this->ch_FOM->Series["Series1"]->Points->Clear();
	this->tb_updates->Text = "FOM chart cleared.";
	Application::DoEvents();
}

private: System::Void closeCOMToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	
	if (!this->serialPort1->IsOpen)
	{
		this->tb_updates->Text = (this->comboBox1->Text) + " is already closed.";
		return;
	}
		
	this->serialPort1->Close();
	this->tb_updates->Text = (this->comboBox1->Text) + " has been closed.";
	return;
}

private: System::Void restartCOMToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	/* scan for com ports again, fill the combobox */
	this->tb_updates->Text = "Closing any open ports...";
	if (this->serialPort1->IsOpen)
	{
		this->serialPort1->Close();
		this->tb_updates->Text = "Port closed.";
	}

	this->comboBox1->Items->Clear();
	findPorts();
	
	this->tb_updates->Text = "Please re-select a port.";
	return;
}
private: System::Void restartToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (this->serialPort1->IsOpen)
	{
		this->serialPort1->Close();
	}

	/* shut down this instance of the application and start a new one */
	Application::Restart();
}//eorestart

private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (this->serialPort1->IsOpen)
	{
		this->serialPort1->Close();
	}

	/* exit the application */
	Application::Exit();
}//eoexit

private: System::Void addEnergyCutsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	/* This function plots lines horizontally across the PSD chart to indicate an Energy cut */
	/* This further allows the user to specify what energy range to look at and plot on the energy spectrum chart */
	double psdXmin{ 0.0 };
	double psdXmax{ 0.0 };
	double psdYmin{ 0.0 };
	double psdYmax{ 0.0 };

	psdXmin = this->ch_PSD->ChartAreas[0]->AxisX->Minimum;
	psdXmax = this->ch_PSD->ChartAreas[0]->AxisX->Maximum;
	psdYmin = this->ch_PSD->ChartAreas[0]->AxisY->Minimum;
	psdYmax = this->ch_PSD->ChartAreas[0]->AxisY->Maximum;

	/* Show the set E cuts pop up */
	 setCutsPopUp ^ setECuts = gcnew setCutsPopUp(psdYmin, psdYmax);
	setECuts->ShowDialog();

	/* Retrieve the cuts the user entered and add them to the chart */
	m_energyLowerCut = setECuts->getCutLower();
	m_energyUpperCut = setECuts->getCutUpper();

	this->ch_PSD->Series["series_ECutLower"]->Points->Clear();
	this->ch_PSD->Series["series_ECutUpper"]->Points->Clear();

	if (m_energyLowerCut == 0 && m_energyUpperCut == 0)	//no cuts
	{
		m_energyLowerCut = 0.0;
		m_energyUpperCut = 0.0;
		this->tb_updates->Text = "No cuts chosen.";
		return;
	}

	if (m_energyLowerCut > 0 && m_energyUpperCut > 0)	//both cuts were chosen
	{
		this->ch_PSD->Series["series_ECutLower"]->Points->AddXY(psdXmin, m_energyLowerCut);
		this->ch_PSD->Series["series_ECutLower"]->Points->AddXY(psdXmax, m_energyLowerCut);
		this->ch_PSD->Series["series_ECutUpper"]->Points->AddXY(psdXmin, m_energyUpperCut);
		this->ch_PSD->Series["series_ECutUpper"]->Points->AddXY(psdXmax, m_energyUpperCut);
	}	

	if (m_energyLowerCut == 0 && m_energyUpperCut > 0)	//Just an upper cut
	{
		this->ch_PSD->Series["series_ECutUpper"]->Points->AddXY(psdXmin, m_energyUpperCut);
		this->ch_PSD->Series["series_ECutUpper"]->Points->AddXY(psdXmax, m_energyUpperCut);
	}
		
	if (m_energyLowerCut > 0 && m_energyUpperCut == 0)	//Just a lower cut
	{
		this->ch_PSD->Series["series_ECutLower"]->Points->AddXY(psdXmin, m_energyLowerCut);
		this->ch_PSD->Series["series_ECutLower"]->Points->AddXY(psdXmax, m_energyLowerCut);
	}
}

private: System::Void addFOMCutsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	/* allows user to plot two vertical lines on ch_PSD, indicating cuts which are plotted by FOM chart */
	double psdXmin{ 0.0 };
	double psdXmax{ 0.0 };
	double psdYmin{ 0.0 };
	double psdYmax{ 0.0 };

	psdXmin = this->ch_PSD->ChartAreas[0]->AxisX->Minimum;
	psdXmax = this->ch_PSD->ChartAreas[0]->AxisX->Maximum;
	psdYmin = this->ch_PSD->ChartAreas[0]->AxisY->Minimum;
	psdYmax = this->ch_PSD->ChartAreas[0]->AxisY->Maximum;

	/* Show the set E cuts pop up */
	setCutsPopUp ^ setFOMCuts = gcnew setCutsPopUp(psdXmin, psdXmax);
	setFOMCuts->ShowDialog();

	/* Retrieve the cuts the user entered and add them to the chart */
	m_fomLeftCut = setFOMCuts->getCutLower();
	m_fomRightCut = setFOMCuts->getCutUpper();

	this->ch_PSD->Series["series_FOMCutLeft"]->Points->Clear();	
	this->ch_PSD->Series["series_FOMCutRight"]->Points->Clear();

	if (m_fomLeftCut == 0 && m_fomRightCut == 0)	//no cuts
	{
		m_fomLeftCut = 0.0;
		m_fomRightCut = 0.0;

		this->tb_updates->Text = "No cuts chosen.";
		return;
	}

	if (m_fomLeftCut > 0 && m_fomRightCut > 0)	//both cuts were chosen
	{
		this->ch_PSD->Series["series_FOMCutLeft"]->Points->AddXY(m_fomLeftCut, psdYmin);
		this->ch_PSD->Series["series_FOMCutLeft"]->Points->AddXY(m_fomLeftCut, psdYmax);
		this->ch_PSD->Series["series_FOMCutRight"]->Points->AddXY(m_fomRightCut, psdYmin);
		this->ch_PSD->Series["series_FOMCutRight"]->Points->AddXY(m_fomRightCut, psdYmax);
	}

	if (m_fomLeftCut == 0 && m_fomRightCut > 0)	//Just an upper cut
	{
		this->ch_PSD->Series["series_FOMCutRight"]->Points->AddXY(m_fomRightCut, psdYmin);
		this->ch_PSD->Series["series_FOMCutRight"]->Points->AddXY(m_fomRightCut, psdYmax);
	}

	if (m_fomLeftCut > 0 && m_fomRightCut == 0)	//Just a lower cut
	{
		this->ch_PSD->Series["series_FOMCutLeft"]->Points->AddXY(m_fomLeftCut, psdYmin);
		this->ch_PSD->Series["series_FOMCutLeft"]->Points->AddXY(m_fomLeftCut, psdYmax);
	}
}

private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
{
	//do work
}
private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) 
{
	//update the main thread
}
private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
{
	//what to do on cancel or completion
}
private: System::Void backgroundWorker2_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
{
	BackgroundWorker^ counterWorker = dynamic_cast<BackgroundWorker^>(sender);
	int counter(0);

	while (1)
	{
		Sleep(500);
		counterWorker->ReportProgress(counter);
		counter++;
		if (counter > 9)	//if the counter goes to 10, reset it to 0
			counter = 0;	//ie. we loop 0 - 9 -> 0 - 9
	}
}
private: System::Void backgroundWorker2_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) 
{
	this->tb_counterbox->Text = e->ProgressPercentage + " ";
}
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	
}
};
}
