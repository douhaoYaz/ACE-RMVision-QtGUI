#include "ui.h"
#include <cstring>
#ifdef QTGUI
#include "qtgui.h"
#endif
#ifdef CUI
#include "cui.h"
#endif

UserInterfaceFactory& UserInterfaceFactory::getFactory() {
    static UserInterfaceFactory factory;
    return factory;
}

void UserInterfaceFactory::initialize() {

#ifdef QTGUI
    ui.reset(new QtGraphicalUserInterface());
#endif
#ifdef CUI
    ui.reset(new CommandUserInterface());
#endif
#ifdef NOUI
    ui.reset(new NoUserInterface());
#endif
}

std::shared_ptr<UserInterface> UserInterfaceFactory::get() {
    return ui;
}

UserInterfaceFactory::UserInterfaceFactory(): ui(nullptr) { }


