cmake_minimum_required(VERSION 3.16)

project(Armazem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(MINGW)
    set(CMAKE_AUTOMOC_COMPILER_PREDEFINES OFF)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql LinguistTools)

set(TS_FILES Armazem_pt_BR.ts)

set(CORE_SOURCES
    Core/AppContext.cpp
    Core/ApplicationBootstrap.cpp
    Core/SessionManager.h
    Core/SessionManager.cpp
)

set(DATABASE_SOURCES
    Database/DatabaseManager.h
    Database/DatabaseManager.cpp
    Database/SchemaInitializer.h
    Database/SchemaInitializer.cpp
)

set(DAO_SOURCES
    DAO/UsuarioDAO.h
    DAO/UsuarioDAO.cpp
    DAO/CategoriaDAO.h
    DAO/CategoriaDAO.cpp
    DAO/ProdutoDAO.h
    DAO/ProdutoDAO.cpp
    DAO/VendaDAO.h
    DAO/VendaDAO.cpp
    DAO/MovimentacaoDAO.h
    DAO/MovimentacaoDAO.cpp
    DAO/LogDAO.h
    DAO/LogDAO.cpp
)
set(MODEL_SOURCES
    Models/Enums.h
    Models/Enums.cpp
    Models/Produto.h
    Models/Usuario.h
)

set(SERVICE_SOURCES
    Services/AuthService.h
    Services/AuthService.cpp
    Services/UserService.h
    Services/UserService.cpp
    Services/ProductService.h
    Services/ProductService.cpp
    Services/InventoryService.h
    Services/InventoryService.cpp
    Services/SalesService.h
    Services/SalesService.cpp
)

set(UTIL_SOURCES
    Utils/PasswordUtils.h
    Utils/ValidationUtils.h
    Utils/ValidationUtils.cpp
)

set(WINDOW_SOURCES
    Windows/loginwindow.h
    Windows/loginwindow.cpp
    Windows/loginwindow.ui
    Windows/cadastroadminwindow.h
    Windows/cadastroadminwindow.cpp
    Windows/cadastroadminwindow.ui
    Windows/dashboardwindow.h
    Windows/dashboardwindow.cpp
    Windows/dashboardwindow.ui
    Windows/cadastrousuariowindow.h
    Windows/cadastrousuariowindow.cpp
    Windows/cadastrousuariowindow.ui
    Windows/gerenciarusuariowindow.h
    Windows/gerenciarusuariowindow.cpp
    Windows/gerenciarusuariowindow.ui
    Windows/cadastroprodutowindow.h
    Windows/cadastroprodutowindow.cpp
    Windows/cadastroprodutowindow.ui
    Windows/gerenciarprodutoswindow.h
    Windows/gerenciarprodutoswindow.cpp
    Windows/gerenciarprodutoswindow.ui
    Windows/movimentacaoestoquewindow.h
    Windows/movimentacaoestoquewindow.cpp
    Windows/movimentacaoestoquewindow.ui
    Windows/registrovendawindow.h
    Windows/registrovendawindow.cpp
    Windows/registrovendawindow.ui
    Windows/consultaprecowindow.h
    Windows/consultaprecowindow.cpp
    Windows/consultaprecowindow.ui
)

set(PROJECT_SOURCES
    main.cpp
    ${CORE_SOURCES}
    ${DATABASE_SOURCES}
    ${DAO_SOURCES}
    ${MODEL_SOURCES}
    ${SERVICE_SOURCES}
    ${UTIL_SOURCES}
    ${WINDOW_SOURCES}
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Armazem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Armazem APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Armazem SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Armazem
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(Armazem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Armazem PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Armazem)
endif()
set_target_properties(Armazem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Armazem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Armazem)
endif()
