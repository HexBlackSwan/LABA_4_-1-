#include "MainForm.h"
#include "EditRecordForm.h"

#include <msclr/marshal_cppstd.h>
#include <algorithm>

namespace Laba4App {

    MainForm::MainForm()
        : manager_(new RecordManager()) {
        InitializeComponent();
        RefreshGrid();
    }

    MainForm::~MainForm() {
        delete manager_;
        manager_ = nullptr;
    }

    void MainForm::InitializeComponent() {
        this->Text = "ЛР4: Учёт успеваемости";
        this->Width = 900;
        this->Height = 560;
        this->StartPosition = FormStartPosition::CenterScreen;

        grid = gcnew DataGridView();
        grid->Left = 20;
        grid->Top = 20;
        grid->Width = 840;
        grid->Height = 360;
        grid->ReadOnly = true;
        grid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        grid->AllowUserToAddRows = false;
        grid->MultiSelect = false;
        grid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        grid->Columns->Add("id", "ID");
        grid->Columns->Add("name", "Имя");
        grid->Columns->Add("group", "Группа");
        grid->Columns->Add("score", "Оценка");

        btnLoad = gcnew Button();
        btnLoad->Left = 20;
        btnLoad->Top = 400;
        btnLoad->Width = 130;
        btnLoad->Text = "Загрузить";
        btnLoad->Click += gcnew EventHandler(this, &MainForm::OnLoadClick);

        btnSave = gcnew Button();
        btnSave->Left = 160;
        btnSave->Top = 400;
        btnSave->Width = 130;
        btnSave->Text = "Сохранить";
        btnSave->Click += gcnew EventHandler(this, &MainForm::OnSaveClick);

        btnAdd = gcnew Button();
        btnAdd->Left = 300;
        btnAdd->Top = 400;
        btnAdd->Width = 130;
        btnAdd->Text = "Добавить";
        btnAdd->Click += gcnew EventHandler(this, &MainForm::OnAddClick);

        btnEdit = gcnew Button();
        btnEdit->Left = 440;
        btnEdit->Top = 400;
        btnEdit->Width = 130;
        btnEdit->Text = "Изменить";
        btnEdit->Click += gcnew EventHandler(this, &MainForm::OnEditClick);

        btnDelete = gcnew Button();
        btnDelete->Left = 580;
        btnDelete->Top = 400;
        btnDelete->Width = 130;
        btnDelete->Text = "Удалить";
        btnDelete->Click += gcnew EventHandler(this, &MainForm::OnDeleteClick);

        cbSort = gcnew ComboBox();
        cbSort->Left = 20;
        cbSort->Top = 440;
        cbSort->Width = 180;
        cbSort->DropDownStyle = ComboBoxStyle::DropDownList;
        cbSort->Items->Add("По ID");
        cbSort->Items->Add("По имени");
        cbSort->Items->Add("По оценке (убыв.)");
        cbSort->SelectedIndex = 0;

        btnSort = gcnew Button();
        btnSort->Left = 210;
        btnSort->Top = 440;
        btnSort->Width = 130;
        btnSort->Text = "Сортировать";
        btnSort->Click += gcnew EventHandler(this, &MainForm::OnSortClick);

        lblSummary = gcnew Label();
        lblSummary->Left = 360;
        lblSummary->Top = 445;
        lblSummary->Width = 500;
        lblSummary->Height = 40;
        lblSummary->Font = gcnew Drawing::Font("Segoe UI", 9, Drawing::FontStyle::Bold);

        openFileDialog = gcnew OpenFileDialog();
        openFileDialog->Filter = "CSV Files (*.csv)|*.csv|Text Files (*.txt)|*.txt|All Files (*.*)|*.*";

        saveFileDialog = gcnew SaveFileDialog();
        saveFileDialog->Filter = "CSV Files (*.csv)|*.csv|Text Files (*.txt)|*.txt|All Files (*.*)|*.*";

        this->Controls->Add(grid);
        this->Controls->Add(btnLoad);
        this->Controls->Add(btnSave);
        this->Controls->Add(btnAdd);
        this->Controls->Add(btnEdit);
        this->Controls->Add(btnDelete);
        this->Controls->Add(cbSort);
        this->Controls->Add(btnSort);
        this->Controls->Add(lblSummary);
    }

    void MainForm::RefreshGrid() {
        grid->Rows->Clear();

        for (const auto& record : manager_->GetAll()) {
            grid->Rows->Add(record.id, gcnew String(record.name.c_str()), gcnew String(record.group.c_str()), String::Format("{0:F2}", record.score));
        }

        UpdateSummary();
    }

    bool MainForm::TryGetSelectedId(int% id) {
        if (grid->SelectedRows->Count == 0) {
            MessageBox::Show("Выберите строку для выполнения операции.", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return false;
        }

        Object^ value = grid->SelectedRows[0]->Cells[0]->Value;
        if (value == nullptr || !Int32::TryParse(value->ToString(), id)) {
            MessageBox::Show("Не удалось определить ID выбранной записи.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return false;
        }

        return true;
    }

    void MainForm::UpdateSummary() {
        Record best{};
        double average = manager_->GetAverageScore();
        if (!manager_->GetBestRecord(best)) {
            lblSummary->Text = "Нет данных. Добавьте записи вручную или загрузите файл.";
            return;
        }

        lblSummary->Text = String::Format(
            "Средний балл: {0:F2} | Лучший: {1} ({2:F2})",
            average,
            gcnew String(best.name.c_str()),
            best.score);
    }

    void MainForm::OnLoadClick(Object^ sender, EventArgs^ e) {
        if (openFileDialog->ShowDialog() != ::DialogResult::OK) {
            return;
        }

        std::string error;
        std::string path = msclr::interop::marshal_as<std::string>(openFileDialog->FileName);
        if (!manager_->LoadFromFile(path, error)) {
            MessageBox::Show(gcnew String(error.c_str()), "Ошибка загрузки", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        RefreshGrid();
    }

    void MainForm::OnSaveClick(Object^ sender, EventArgs^ e) {
        if (saveFileDialog->ShowDialog() != ::DialogResult::OK) {
            return;
        }

        std::string error;
        std::string path = msclr::interop::marshal_as<std::string>(saveFileDialog->FileName);
        if (!manager_->SaveToFile(path, error)) {
            MessageBox::Show(gcnew String(error.c_str()), "Ошибка сохранения", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        MessageBox::Show("Данные успешно сохранены.", "Сохранение", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void MainForm::OnAddClick(Object^ sender, EventArgs^ e) {
        EditRecordForm^ dialog = gcnew EditRecordForm();
        if (dialog->ShowDialog(this) != ::DialogResult::OK) {
            return;
        }

        Record record{};
        String^ validationError;
        if (!dialog->TryBuildRecord(record, validationError)) {
            MessageBox::Show(validationError, "Ошибка ввода", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        std::string error;
        if (!manager_->Add(record, error)) {
            MessageBox::Show(gcnew String(error.c_str()), "Ошибка добавления", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        RefreshGrid();
    }

    void MainForm::OnEditClick(Object^ sender, EventArgs^ e) {
        int id = 0;
        if (!TryGetSelectedId(id)) {
            return;
        }

        const auto& all = manager_->GetAll();
        auto it = std::find_if(all.begin(), all.end(), [id](const Record& record) { return record.id == id; });
        if (it == all.end()) {
            MessageBox::Show("Выбранная запись не найдена.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        EditRecordForm^ dialog = gcnew EditRecordForm(*it);
        if (dialog->ShowDialog(this) != ::DialogResult::OK) {
            return;
        }

        Record updated{};
        String^ validationError;
        if (!dialog->TryBuildRecord(updated, validationError)) {
            MessageBox::Show(validationError, "Ошибка ввода", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        std::string error;
        if (!manager_->Update(id, updated, error)) {
            MessageBox::Show(gcnew String(error.c_str()), "Ошибка изменения", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        RefreshGrid();
    }

    void MainForm::OnDeleteClick(Object^ sender, EventArgs^ e) {
        int id = 0;
        if (!TryGetSelectedId(id)) {
            return;
        }

        if (MessageBox::Show("Удалить выбранную запись?", "Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Question) != ::DialogResult::Yes) {
            return;
        }

        std::string error;
        if (!manager_->Remove(id, error)) {
            MessageBox::Show(gcnew String(error.c_str()), "Ошибка удаления", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        RefreshGrid();
    }

    void MainForm::OnSortClick(Object^ sender, EventArgs^ e) {
        switch (cbSort->SelectedIndex) {
        case 0:
            manager_->SortById();
            break;
        case 1:
            manager_->SortByName();
            break;
        case 2:
            manager_->SortByScoreDesc();
            break;
        default:
            MessageBox::Show("Выберите корректный вариант сортировки.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        RefreshGrid();
    }
}
