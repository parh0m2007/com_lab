#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QSplitter>
#include "graphwidget.h"

/**
 * @class MainWindow
 * @brief Main application window for the TSP Branch and Bound solver.
 * 
 * This class provides the complete user interface including:
 * - Menu bar with File, Edit, Mode, and Help menus
 * - Toolbar with quick access buttons
 * - Graph visualization widget
 * - Console/output panel for displaying results and accepting input
 * - Status bar with information
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    
private slots:
    /**
     * @brief Create a new empty graph
     */
    void newGraph();
    
    /**
     * @brief Load the sample demonstration graph
     */
    void loadSample();
    
    /**
     * @brief Clear the current graph
     */
    void clearGraph();
    
    /**
     * @brief Solve the TSP problem
     */
    void solveTSP();
    
    /**
     * @brief Set mode to Add Node
     */
    void setAddNodeMode();
    
    /**
     * @brief Set mode to Add Edge
     */
    void setAddEdgeMode();
    
    /**
     * @brief Set mode to Move Node
     */
    void setMoveNodeMode();
    
    /**
     * @brief Set mode to Select
     */
    void setSelectMode();
    
    /**
     * @brief Handle solution found signal
     */
    void onSolutionFound(const std::vector<int>& path, double cost, int nodesExplored);
    
    /**
     * @brief Handle no solution signal
     */
    void onNoSolution();
    
    /**
     * @brief Show about dialog
     */
    void showAbout();
    
    /**
     * @brief Export graph data to console
     */
    void exportData();

private:
    /**
     * @brief Create the menu bar
     */
    void createMenus();
    
    /**
     * @brief Create the toolbar
     */
    void createToolBar();
    
    /**
     * @brief Create the status bar
     */
    void createStatusBar();
    
    /**
     * @brief Create the central widget layout
     */
    void createCentralWidget();
    
    GraphWidget* m_graphWidget;       ///< Main graph visualization widget
    QTextEdit* m_console;             ///< Console/output text area
    QSplitter* m_splitter;            ///< Splitter for resizable panels
    
    // Menu actions
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_modeMenu;
    QMenu* m_helpMenu;
    
    QAction* m_newAction;
    QAction* m_loadSampleAction;
    QAction* m_clearAction;
    QAction* m_exportAction;
    QAction* m_exitAction;
    
    QAction* m_addNodeAction;
    QAction* m_addEdgeAction;
    QAction* m_moveNodeAction;
    QAction* m_selectAction;
    QAction* m_solveAction;
    
    QAction* m_aboutAction;
};

#endif // MAINWINDOW_H
