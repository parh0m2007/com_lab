#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QActionGroup>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Task of the Traveling Salesman - Branch and Bound Method");
    setMinimumSize(1000, 700);
    
    createMenus();
    createToolBar();
    createCentralWidget();
    createStatusBar();
    
    // Load sample graph on startup for demonstration
    loadSample();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
    // File menu
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    
    m_newAction = m_fileMenu->addAction(tr("&New Graph"));
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newGraph);
    
    m_loadSampleAction = m_fileMenu->addAction(tr("&Load Sample Graph"));
    m_loadSampleAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    connect(m_loadSampleAction, &QAction::triggered, this, &MainWindow::loadSample);
    
    m_clearAction = m_fileMenu->addAction(tr("&Clear Graph"));
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::clearGraph);
    
    m_fileMenu->addSeparator();
    
    m_exportAction = m_fileMenu->addAction(tr("&Export Data"));
    m_exportAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportData);
    
    m_fileMenu->addSeparator();
    
    m_exitAction = m_fileMenu->addAction(tr("E&xit"));
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    // Edit menu
    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    
    m_addNodeAction = m_editMenu->addAction(tr("&Add Node Mode"));
    m_addNodeAction->setCheckable(true);
    m_addNodeAction->setChecked(true);
    m_addNodeAction->setShortcut(QKeySequence(Qt::Key_A));
    connect(m_addNodeAction, &QAction::triggered, this, &MainWindow::setAddNodeMode);
    
    m_addEdgeAction = m_editMenu->addAction(tr("Add &Edge Mode"));
    m_addEdgeAction->setCheckable(true);
    m_addEdgeAction->setShortcut(QKeySequence(Qt::Key_E));
    connect(m_addEdgeAction, &QAction::triggered, this, &MainWindow::setAddEdgeMode);
    
    m_moveNodeAction = m_editMenu->addAction(tr("&Move Node Mode"));
    m_moveNodeAction->setCheckable(true);
    m_moveNodeAction->setShortcut(QKeySequence(Qt::Key_M));
    connect(m_moveNodeAction, &QAction::triggered, this, &MainWindow::setMoveNodeMode);
    
    m_selectAction = m_editMenu->addAction(tr("&Select Mode"));
    m_selectAction->setCheckable(true);
    m_selectAction->setShortcut(QKeySequence(Qt::Key_S));
    connect(m_selectAction, &QAction::triggered, this, &MainWindow::setSelectMode);
    
    m_editMenu->addSeparator();
    
    m_solveAction = m_editMenu->addAction(tr("&Solve TSP"));
    m_solveAction->setShortcut(QKeySequence(Qt::Key_Return));
    connect(m_solveAction, &QAction::triggered, this, &MainWindow::solveTSP);
    
    // Mode menu (quick access)
    m_modeMenu = menuBar()->addMenu(tr("&Mode"));
    
    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->addAction(m_addNodeAction);
    modeGroup->addAction(m_addEdgeAction);
    modeGroup->addAction(m_moveNodeAction);
    modeGroup->addAction(m_selectAction);
    
    // Help menu
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    
    m_aboutAction = m_helpMenu->addAction(tr("&About"));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createToolBar()
{
    QToolBar* toolBar = addToolBar(tr("Main Toolbar"));
    toolBar->setMovable(false);
    
    toolBar->addAction(m_newAction);
    toolBar->addAction(m_loadSampleAction);
    toolBar->addAction(m_clearAction);
    toolBar->addSeparator();
    
    toolBar->addAction(m_addNodeAction);
    toolBar->addAction(m_addEdgeAction);
    toolBar->addAction(m_moveNodeAction);
    toolBar->addAction(m_selectAction);
    toolBar->addSeparator();
    
    // Add solve button with prominent styling
    QPushButton* solveButton = new QPushButton("Solve TSP");
    solveButton->setStyleSheet(
        "QPushButton { "
        "background-color: #4CAF50; "
        "color: white; "
        "font-weight: bold; "
        "padding: 5px 15px; "
        "border-radius: 3px; "
        "} "
        "QPushButton:hover { "
        "background-color: #45a049; "
        "}"
    );
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::solveTSP);
    toolBar->addWidget(solveButton);
}

void MainWindow::createStatusBar()
{
    QStatusBar* status = statusBar();
    status->showMessage(tr("Ready - Load a sample graph or create your own"));
}

void MainWindow::createCentralWidget()
{
    m_splitter = new QSplitter(Qt::Vertical);
    
    // Graph widget (top)
    m_graphWidget = new GraphWidget();
    connect(m_graphWidget, &GraphWidget::graphChanged, [this]() {
        statusBar()->showMessage("Graph modified", 2000);
    });
    connect(m_graphWidget, &GraphWidget::solutionFound, 
            this, &MainWindow::onSolutionFound);
    connect(m_graphWidget, &GraphWidget::noSolutionFound,
            this, &MainWindow::onNoSolution);
    
    // Console panel (bottom)
    m_console = new QTextEdit();
    m_console->setReadOnly(true);
    m_console->setMaximumHeight(200);
    m_console->setFont(QFont("Consolas", 10));
    
    m_splitter->addWidget(m_graphWidget);
    m_splitter->addWidget(m_console);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);
    
    setCentralWidget(m_splitter);
}

void MainWindow::newGraph()
{
    m_graphWidget->clearGraph();
    m_console->clear();
    m_console->append("New empty graph created.");
    statusBar()->showMessage("New graph created", 2000);
}

void MainWindow::loadSample()
{
    m_graphWidget->loadSampleGraph();
    m_console->clear();
    m_console->append("Sample graph loaded with 8 cities arranged in a circular pattern.");
    m_console->append("Each city is connected to its neighbors and has cross connections.");
    m_console->append("Click 'Solve TSP' or press Enter to find the optimal route.");
    statusBar()->showMessage("Sample graph loaded", 2000);
}

void MainWindow::clearGraph()
{
    m_graphWidget->clearGraph();
    m_console->append("Graph cleared.");
}

void MainWindow::solveTSP()
{
    m_console->append("\n--- Solving TSP using Branch and Bound ---");
    m_graphWidget->solveTSP();
}

void MainWindow::setAddNodeMode()
{
    m_graphWidget->setMode(GraphWidget::Mode::AddNode);
    m_addNodeAction->setChecked(true);
    statusBar()->showMessage("Mode: Add Node - Click anywhere to add a new city", 3000);
}

void MainWindow::setAddEdgeMode()
{
    m_graphWidget->setMode(GraphWidget::Mode::AddEdge);
    m_addEdgeAction->setChecked(true);
    statusBar()->showMessage("Mode: Add Edge - Click two cities to connect them", 3000);
}

void MainWindow::setMoveNodeMode()
{
    m_graphWidget->setMode(GraphWidget::Mode::MoveNode);
    m_moveNodeAction->setChecked(true);
    statusBar()->showMessage("Mode: Move Node - Drag cities to reposition them", 3000);
}

void MainWindow::setSelectMode()
{
    m_graphWidget->setMode(GraphWidget::Mode::Select);
    m_selectAction->setChecked(true);
    statusBar()->showMessage("Mode: Select - Click to select, Delete to remove", 3000);
}

void MainWindow::onSolutionFound(const std::vector<int>& path, double cost, int nodesExplored)
{
    m_console->append(QString("\n✓ Solution Found!"));
    m_console->append(QString("Total Cost: %1").arg(cost, 0, 'f', 2));
    m_console->append(QString("Nodes Explored: %1").arg(nodesExplored));
    
    // Build path string
    QString pathStr;
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) pathStr += " → ";
        pathStr += QString::number(path[i] + 1);
    }
    pathStr += " → " + QString::number(path[0] + 1); // Return to start
    
    m_console->append(QString("Optimal Path: %1").arg(pathStr));
    m_console->append("------------------------------------------------\n");
    
    statusBar()->showMessage(QString("Solution found! Cost: %1").arg(cost, 0, 'f', 2), 5000);
}

void MainWindow::onNoSolution()
{
    m_console->append("\n✗ No solution found!");
    m_console->append("The graph may not have a valid Hamiltonian cycle.");
    m_console->append("Make sure all cities are connected properly.\n");
    
    statusBar()->showMessage("No solution found", 3000);
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About TSP Solver"),
        tr("<h2>Traveling Salesman Problem Solver</h2>"
           "<p><b>Method:</b> Branch and Bound</p>"
           "<p><b>Technologies:</b></p>"
           "<ul>"
           "<li>C++17</li>"
           "<li>Qt Framework (Widgets & QPainter)</li>"
           "<li>CMake Build System</li>"
           "</ul>"
           "<p>This application demonstrates the Branch and Bound algorithm "
           "for solving the classic Traveling Salesman Problem.</p>"
           "<p><b>Features:</b></p>"
           "<ul>"
           "<li>Interactive graph creation and editing</li>"
           "<li>Visual representation of nodes and edges</li>"
           "<li>Real-time solution visualization</li>"
           "<li>Support for custom graphs with 6+ vertices</li>"
           "</ul>"));
}

void MainWindow::exportData()
{
    m_console->append("\n--- Graph Data Export ---");
    
    const auto& graph = m_graphWidget->graph();
    
    m_console->append(QString("Number of Nodes: %1").arg(graph.nodeCount()));
    m_console->append(QString("Number of Edges: %1").arg(graph.edgeCount()));
    
    m_console->append("\nNodes:");
    for (const auto& node : graph.nodes()) {
        QPointF pos = node->position();
        m_console->append(QString("  Node %1: (%2, %3)")
            .arg(node->label())
            .arg(pos.x(), 0, 'f', 1)
            .arg(pos.y(), 0, 'f', 1));
    }
    
    m_console->append("\nEdges:");
    for (const auto& edge : graph.edges()) {
        m_console->append(QString("  %1 — %2 (weight: %3)")
            .arg(edge->startNode()->label())
            .arg(edge->endNode()->label())
            .arg(edge->weight(), 0, 'f', 2));
    }
    
    m_console->append("------------------------\n");
}
