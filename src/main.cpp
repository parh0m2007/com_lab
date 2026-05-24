#include <QApplication>
#include "mainwindow.h"

/**
 * @mainpage Traveling Salesman Problem - Branch and Bound Solver
 * 
 * This application demonstrates the solution of the classic Traveling Salesman Problem (TSP)
 * using the Branch and Bound algorithm. Built with C++17 and Qt framework.
 * 
 * @section features Features
 * - Interactive graph creation and manipulation
 * - OpenGL-based visualization
 * - Real-time TSP solution display
 * - Support for custom graphs with 6+ vertices
 * 
 * @section usage Usage
 * 1. Load a sample graph or create your own
 * 2. Add nodes by clicking in Add Node mode
 * 3. Connect nodes in Add Edge mode
 * 4. Click "Solve TSP" to find the optimal route
 */

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application info
    QCoreApplication::setApplicationName("TSP Solver");
    QCoreApplication::setApplicationVersion("1.0");
    QCoreApplication::setOrganizationName("Lab Work");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
