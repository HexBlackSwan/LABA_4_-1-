#pragma once

#include "RecordManager.h"

namespace Laba4App {

    using namespace System;
    using namespace System::Windows::Forms;

    public ref class MainForm : public Form {
    public:
        MainForm();
        ~MainForm();

    private:
        RecordManager* manager_;

        DataGridView^ grid;
        Button^ btnLoad;
        Button^ btnSave;
        Button^ btnAdd;
        Button^ btnEdit;
        Button^ btnDelete;
        Button^ btnSort;
        ComboBox^ cbSort;
        Label^ lblSummary;

        OpenFileDialog^ openFileDialog;
        SaveFileDialog^ saveFileDialog;

        void InitializeComponent();
        void RefreshGrid();
        bool TryGetSelectedId(int% id);
        void UpdateSummary();

        void OnLoadClick(Object^ sender, EventArgs^ e);
        void OnSaveClick(Object^ sender, EventArgs^ e);
        void OnAddClick(Object^ sender, EventArgs^ e);
        void OnEditClick(Object^ sender, EventArgs^ e);
        void OnDeleteClick(Object^ sender, EventArgs^ e);
        void OnSortClick(Object^ sender, EventArgs^ e);
    };
}
