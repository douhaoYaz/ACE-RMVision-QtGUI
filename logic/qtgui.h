#ifndef QTGUI_H
#define QTGUI_H

#include "interface.h"

class QtGraphicalUserInterface: public UserInterface {
public:
    QtGraphicalUserInterface();

    void log(const char* str) override;

    void registerImage(const char* name_title, cv::Mat* img, int type) override;

    void registerDataTable(const char* name_table, const std::vector<const char*>& keys,
            const std::vector<Pointer>& ptrs) override;

    void setCurrentTable(const char* name) override;

    void createEditItemBar(const char* name) override;

    void registerTrackBar(const char* name_bar, const char* name,
            int* data, int min, int max) override;

};

#endif // QTGUI_H
