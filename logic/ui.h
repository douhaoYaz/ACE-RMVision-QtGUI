#ifndef UI_H
#define UI_H

#include "interface.h"
#include <memory>

class NoUserInterface: public UserInterface {
public:
    NoUserInterface() { }

    void log(const char*) override { }

    void registerImage(const char*, cv::Mat*, int) override { }

    void registerDataTable(const char*, const std::vector<const char*>&,
            const std::vector<Pointer>&) override { }

    void setCurrentTable(const char*) override { }

    void createEditItemBar(const char*) override { }

    void registerTrackBar(const char*, const char*, int*, int, int) override { }

};

class UserInterfaceFactory {
public:
    static UserInterfaceFactory& getFactory();

    void initialize();

    std::shared_ptr<UserInterface> get();

private:
    std::shared_ptr<UserInterface> ui;

    UserInterfaceFactory();

};

#endif // USERINTERFACE_H
