#pragma once

#include "Record.h"

namespace Laba4App {

    using namespace System;
    using namespace System::Windows::Forms;

    public ref class EditRecordForm : public Form {
    public:
        EditRecordForm();
        EditRecordForm(const Record& record);

        bool TryBuildRecord(Record& outRecord, String^% error);

    private:
        bool isEditMode_;

        Label^ lblHeader;
        Label^ lblId;
        Label^ lblName;
        Label^ lblGroup;
        Label^ lblScore;

        TextBox^ tbId;
        TextBox^ tbName;
        TextBox^ tbGroup;
        TextBox^ tbScore;

        Button^ btnOk;
        Button^ btnCancel;

        void InitializeComponent();
        void ApplyVisualMode();
    };
}
