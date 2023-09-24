#ifndef QTMESSAGELOGWIDGET_H
#define QTMESSAGELOGWIDGET_H
#include <QWidget>
#include <QScopedPointer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#if QT_VERSION < 0x050000
#include <QtGui/QAbstractScrollArea>
#else
#include <QtWidgets/QAbstractScrollArea>

#endif

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
class QColor;
class QRegularExpression;
QT_END_NAMESPACE

class  QtMessageLogWidget :
        public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY( uint historySize READ historySize WRITE setHistorySize )
    Q_PROPERTY( QString text READ text )
    Q_PROPERTY( uint minimumVisibleLines READ minimumVisibleLines WRITE setMinimumVisibleLines )
    Q_PROPERTY( uint minimumVisibleColumns READ minimumVisibleColumns WRITE setMinimumVisibleColumns )
    Q_PROPERTY( bool alternatingRowColors READ alternatingRowColors WRITE setAlternatingRowColors )

public:
    /*!
     * Constructor. Creates an empty QtMessageLogWidget.
     */
    explicit QtMessageLogWidget( QWidget * parent = Q_NULLPTR );
    /*!
     * Destructor.
     */
    ~QtMessageLogWidget();


    /*!
     * \property QtMessageLogWidget::historySize
     *
     * Specifies the maximum number of lines this widget will hold before
     * dropping old lines. The default is INT_MAX (ie. essentially unlimited).
     * Get this property's value using %historySize(), and set it with
     * %setHistorySize().
     */
    void setHistorySize( unsigned int size );
    unsigned int historySize() const;

    /*!
     * \property QtMessageLogWidget::text
     *
     * Contains the current %text as a single string.
     */
    QString text() const;

    /*!
     * \property QtMessageLogWidget::minimumVisibleLines
     * Specifies the number of lines that should be visible at any one
     * time. The default is 1 (one).
     * Get this property's value using %minimumVisibleLines(), and set it
     * using %setMinimumVisibleLines().
     */
    void setMinimumVisibleLines( unsigned int num );
    unsigned int minimumVisibleLines() const;
    /*!
     * \property QtMessageLogWidget::minimumVisibleColumns
     *
     * Specifies the number of columns that should be visible at any one
     * time. The default is 1 (one). The width is calculated using
     * QFontMetrics::averageCharWidth(), if that is available. Otherwise,
     * the width of \c M is used.
     * Get this property's value using %minimumVisibleColumns(), and set it
     * using %setMinimumVisibleColumns().
     */
    void setMinimumVisibleColumns( unsigned int num );
    unsigned int minimumVisibleColumns() const;

    /*!
     * \property QtMessageLogWidget::alternatingRowColors
     *
     * Specifies whether the background should be drawn using
     * row-alternating colors. The default is \c false.
     * Get this property's value using %alternatingRowColors(), and set it
     * using %setAlternatingRowColors().
     */
    void setAlternatingRowColors( bool on );
    bool alternatingRowColors() const;

    int setupStyle(const QRegularExpression& expr, const QColor& background, const QColor& foreground);
    int removeStyle(int id);
    void clearStyles();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    
signals:
    void updateCursorPosHeight(int height);

private:
    /*bool isAjustHieght = false;
    int CursorDeltaHeight = 0;*/
public Q_SLOTS:
    /*!
     * Clears the text.
     */
    void clear();
    /*!
     * Appends \a str to the view, highlighting the line
     */
    virtual void message( const QString & msg );

    void message(const QString& msg, const QString color);

/*protected:
    virtual void highlight( const QString& text );*/

    // QAbstractScrollArea interface
protected:
    virtual void scrollContentsBy(int dx, int dy);

    // QWidget interface
protected:
    virtual void paintEvent( QPaintEvent * ) Q_DECL_OVERRIDE;
    virtual void timerEvent( QTimerEvent * ) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent * ) Q_DECL_OVERRIDE;
    virtual void changeEvent( QEvent * ) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent * ) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
private:
    QScopedPointer<class QtMessageLogWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMessageLogWidget)
};

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    LogWidget(QWidget* parent = nullptr);
    ~LogWidget();

public:
    void SetLogWidgetLayout();
    void message(const QString& msg, const QString color);
private:
    void CreateToolBar();

public slots:
    void changeLogWidgetHeightSlot();

signals:
    void changeLogWidgetHeight(int);
private:
    //QHBoxLayout* m_qHBoxLayout;
    QVBoxLayout* m_qVBoxLayout;
    QtMessageLogWidget* m_logWidget;
    QSlider* changeLogHeight;
};



#endif // QTMESSAGELOGWIDGET_H
