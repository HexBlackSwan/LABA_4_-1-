#include "EditRecordForm.h"

#include <msclr/marshal_cppstd.h>

namespace Laba4App {

    EditRecordForm::EditRecordForm()
        : isEditMode_(false) {
        InitializeComponent();
        ApplyVisualMode();
    }

    EditRecordForm::EditRecordForm(const Record& record)
        : isEditMode_(true) {
        InitializeComponent();

        tbId->Text = Convert::ToString(record.id);
        tbName->Text = gcnew String(record.name.c_str());
        tbGroup->Text = gcnew String(record.group.c_str());
        tbScore->Text = String::Format("{0:F2}", record.score);

        ApplyVisualMode();
    }

    bool EditRecordForm::TryBuildRecord(Record& outRecord, String^% error) {
        error = nullptr;

        int parsedId = 0;
        if (!Int32::TryParse(tbId->Text->Trim(), parsedId) || parsedId <= 0) {
            error = "ID должен быть положительным целым числом.";
            return false;
        }

        String^ name = tbName->Text->Trim();
        String^ group = tbGroup->Text->Trim();
        if (String::IsNullOrWhiteSpace(name) || String::IsNullOrWhiteSpace(group)) {
            error = "Имя и группа обязательны для заполнения.";
            return false;
        }

        double parsedScore = 0.0;
        if (!Double::TryParse(tbScore->Text->Trim(), parsedScore) || parsedScore < 0.0 || parsedScore > 100.0) {
            error = "Оценка должна быть числом от 0 до 100.";
            return false;
        }

        outRecord.id = parsedId;
        outRecord.name = msclr::interop::marshal_as<std::string>(name);
        outRecord.group = msclr::interop::marshal_as<std::string>(group);
        outRecord.score = parsedScore;
        return true;
    }

    void EditRecordForm::InitializeComponent() {
        this->Text = "Запись";
        this->Width = 420;
        this->Height = 320;
        this->FormBorderStyle = FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;
        this->StartPosition = FormStartPosition::CenterParent;

        lblHeader = gcnew Label();
        lblHeader->Left = 20;
        lblHeader->Top = 15;
        lblHeader->Width = 360;
        lblHeader->Height = 24;
        lblHeader->Font = gcnew Drawing::Font("Segoe UI", 11, Drawing::FontStyle::Bold);

        lblId = gcnew Label();
        lblId->Left = 20;
        lblId->Top = 55;
        lblId->Text = "ID:";

        tbId = gcnew TextBox();
        tbId->Left = 130;
        tbId->Top = 52;
        tbId->Width = 240;

        lblName = gcnew Label();
        lblName->Left = 20;
        lblName->Top = 90;
        lblName->Text = "Имя:";

        tbName = gcnew TextBox();
        tbName->Left = 130;
        tbName->Top = 87;
        tbName->Width = 240;

        lblGroup = gcnew Label();
        lblGroup->Left = 20;
        lblGroup->Top = 125;
        lblGroup->Text = "Группа:";

        tbGroup = gcnew TextBox();
        tbGroup->Left = 130;
        tbGroup->Top = 122;
        tbGroup->Width = 240;

        lblScore = gcnew Label();
        lblScore->Left = 20;
        lblScore->Top = 160;
        lblScore->Text = "Оценка [0..100]:";

        tbScore = gcnew TextBox();
        tbScore->Left = 130;
        tbScore->Top = 157;
        tbScore->Width = 240;

        btnOk = gcnew Button();
        btnOk->Left = 130;
        btnOk->Top = 210;
        btnOk->Width = 110;
        btnOk->DialogResult = ::DialogResult::OK;

        btnCancel = gcnew Button();
        btnCancel->Left = 260;
        btnCancel->Top = 210;
        btnCancel->Width = 110;
        btnCancel->Text = "Отмена";
        btnCancel->DialogResult = ::DialogResult::Cancel;

        this->Controls->Add(lblHeader);
        this->Controls->Add(lblId);
        this->Controls->Add(tbId);
        this->Controls->Add(lblName);
        this->Controls->Add(tbName);
        this->Controls->Add(lblGroup);
        this->Controls->Add(tbGroup);
        this->Controls->Add(lblScore);
        this->Controls->Add(tbScore);
        this->Controls->Add(btnOk);
        this->Controls->Add(btnCancel);

        this->AcceptButton = btnOk;
        this->CancelButton = btnCancel;
    }

    void EditRecordForm::ApplyVisualMode() {
        if (isEditMode_) {
            this->Text = "Редактирование записи";
            lblHeader->Text = "Сценарий: изменение существующей записи";
            btnOk->Text = "Сохранить";
            tbId->ReadOnly = true;
            tbId->BackColor = Drawing::Color::Gainsboro;
        }
        else {
            this->Text = "Добавление записи";
            lblHeader->Text = "Сценарий: создание новой записи";
            btnOk->Text = "Добавить";
            tbId->ReadOnly = false;
            tbId->BackColor = Drawing::Color::White;
        }
    }
}
