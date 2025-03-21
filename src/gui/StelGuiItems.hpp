/*
 * Stellarium
 * Copyright (C) 2008 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#ifndef STELGUIITEMS_HPP
#define STELGUIITEMS_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsWidget>
#include <QDebug>
#include <QMap>

class QGraphicsSceneMouseEvent;
class QTimeLine;
class QGraphicsTextItem;
class QTimer;
class StelProgressController;
class QProgressBar;

// Progress bars in the lower right corner
class StelProgressBarMgr : public QGraphicsWidget
{
	Q_OBJECT
public:
	StelProgressBarMgr(QGraphicsItem* parent);
	
public slots:
	void addProgressBar(const StelProgressController *p);
	void removeProgressBar(const StelProgressController *p);
	void oneBarChanged();
private:
	QMap<const StelProgressController*, QProgressBar*> allBars;
};

// Buttons in the bottom left corner
class CornerButtons : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	CornerButtons(QGraphicsItem* parent=nullptr);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF boundingRect() const override;
	void setOpacity(double opacity);
private:
	mutable double lastOpacity;
};

//! A Button Graphicsitem for use in Stellarium's graphic widgets
class StelButton : public QObject, public QGraphicsPixmapItem
{
	friend class BottomStelBar;
	friend class LeftStelBar;
	Q_OBJECT
public:
	//! Constructor
	//! @param parent the parent item
	//! @param pixOn the pixmap to display when the button is toggled
	//! @param pixOff the pixmap to display when the button is not toggled
	//! @param pixHover a pixmap slowly blended when mouse is over the button
	//! @param action the associated action. Connections are automatically done with the signals if relevant.
	//! @param noBackground define whether the button background image have to be used
	StelButton(QGraphicsItem* parent, const QPixmap& pixOn, const QPixmap& pixOff,
		   const QPixmap& pixHover=QPixmap(),
		   class StelAction* action=nullptr,
		   bool noBackground=false,
		   StelAction *otherAction=nullptr);
	
	//! Constructor
	//! @param parent the parent item
	//! @param pixOn the pixmap to display when the button is toggled
	//! @param pixOff the pixmap to display when the button is not toggled
	//! @param pixHover a pixmap slowly blended when mouse is over the button
	//! @param actionId the id of the associated action. Connections are automatically done with the signals if relevant.
	//! @param noBackground define whether the button background image have to be used
	StelButton(QGraphicsItem* parent, const QPixmap& pixOn, const QPixmap& pixOff,
		   const QPixmap& pixHover,
		   const QString& actionId,
		   bool noBackground=false,
		   const QString& otherActionId="");
	//! Constructor
	//! @param parent the parent item
	//! @param pixOn the pixmap to display when the button is toggled
	//! @param pixOff the pixmap to display when the button is not toggled
	//! @param pixNoChange the pixmap to display when the button state of a tristate is not changed
	//! @param pixHover a pixmap slowly blended when mouse is over the button
	//! @param actionId the associated action. Connections are automatically done with the signals if relevant.
	//! @param noBackground define whether the button background image have to be used
	//! @param isTristate define whether the button is a tristate or an on/off button
	StelButton(QGraphicsItem* parent, const QPixmap& pixOn, const QPixmap& pixOff, const QPixmap& pixNoChange,
		   const QPixmap& pixHover,
		   const QString& actionId=QString(),
		   bool noBackground=false,
		   bool isTristate=true);
	
	//! Button states
	enum {ButtonStateOff = 0, ButtonStateOn = 1, ButtonStateNoChange = 2};

	//! Get whether the button is checked
	int isChecked() const {return checked;}

	//! Get the width of the button image.
	//! The width is based on pixOn.
	int getButtonPixmapWidth() const;
	int getButtonPixmapHeight() const;

	//! Set the button opacity
	void setOpacity(double v) {opacity=v; updateIcon();}

	//! Set the background pixmap of the button.
	void setBackgroundPixmap(const QPixmap& newBackground);

	//! While configuring buttons, call this with true when after key release
	//! focus should go back to the sky (typical for bottom buttons;
	//! left buttons call panels which receive focus after button press, so those should be configured with b=false)
	void setFocusOnSky(bool b) { flagChangeFocus=b; }

	//! Configure the button to trigger its action when the mouse click
	//! is released (by default buttons trigger on press event).
	void setTriggerOnRelease(bool b) { triggerOnRelease = b;}

	QRectF boundingRect() const override;

	static double getInputPixmapsDevicePixelRatio() { return GUI_INPUT_PIXMAPS_SCALE; }


signals:
	//! Triggered when the button state changes
	void toggled(bool);
	//! Triggered when the button state changes
	void triggered();
	void triggeredRight();
	//! Emitted when the hover state change
	//! @param b true if the mouse entered the button
	void hoverChanged(bool b);

public slots:
	//! set whether the button is checked
	void setChecked(int b);
	void setChecked(bool b) { setChecked(static_cast<int>(b)); }
	void updateIcon();

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
private slots:
	void animValueChanged(qreal value);
private:
	void initCtor(const QPixmap& apixOn,
                  const QPixmap& apixOff,
                  const QPixmap& apixNoChange,
                  const QPixmap& apixHover,
		  StelAction* anAction,
		  StelAction* otherAction,
		  bool noBackground,
		  bool isTristate);
	int toggleChecked(int);
	static double buttonSizeRatio();

	QPixmap pixOn;
	QPixmap pixOff;
	QPixmap pixNoChange;
	QPixmap pixHover;
	QPixmap pixBackground;
	QPixmap scaledCurrentPixmap;

	int checked;
	bool flagChangeFocus;

	QTimeLine* timeLine;
	class StelAction* action;          // linked action for button press
	class StelAction* secondAction;    // linked action for rightclick (or Ctrl-Click?)
	bool noBckground;
	bool isTristate_;
	double opacity;
	double hoverOpacity;
	bool triggerOnRelease = false;
	// All input pixmaps are supposed to be 500% scaled compared to their
	// appearance on the screen when high-DPI scale factor is 100%
	static constexpr double GUI_INPUT_PIXMAPS_SCALE = 5;
	// This is the actual scale to which the input pixmaps will be resized after loading
	inline static double pixmapsScale = 0;
};

// The button bar on the left containing windows toggle buttons
class LeftStelBar : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	LeftStelBar(QGraphicsItem* parent);
	~LeftStelBar() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF boundingRect() const override;
	void addButton(StelButton* button);
	QRectF boundingRectNoHelpLabel() const;
	//! Set the color for all the sub elements
	void setColor(const QColor& c);
private slots:
	//! Update the help label when a button is hovered
	void buttonHoverChanged(bool b);

	//! connect from StelApp to resize fonts on the fly.
	void setFontSizeFromApp(int size);
	//! connect from StelApp to set font on the fly.
	void setFont(const QFont &cfont);
	void updateButtonPositions();
private:
	QTimeLine* hideTimeLine;
	QGraphicsSimpleTextItem* helpLabel;
};

// The button bar on the bottom containing actions toggle buttons
class BottomStelBar : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	BottomStelBar(QGraphicsItem* parent, const QPixmap& pixLeft=QPixmap(), const QPixmap& pixRight=QPixmap(), const QPixmap& pixMiddle=QPixmap(), const QPixmap& pixSingle=QPixmap());
	~BottomStelBar() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF boundingRect() const override;
	QRectF boundingRectNoHelpLabel() const;

	//! Add a button in a group in the button bar. Group are displayed in alphabetic order.
	//! @param button the button to add
	//! @param groupName the name of the button group to which the button belongs to. If the group doesn't exist yet, a new one is created.
	//! @param beforeActionName insert the button before the button associated to the given action. If the action doesn't exist, insert it at the end of the group.
	void addButton(StelButton* button, const QString& groupName="defaultGroup", const QString& beforeActionName="");
	//! Hide the button associated with the action of the passed name
	StelButton* hideButton(const QString& actionName);

	//! Set the margin at the left and right of a button group in pixels
	void setGroupMargin(const QString& groupName, int left, int right);

	//! Set the background of a group
	void setGroupBackground(const QString& groupName, const QPixmap& pixLeft=QPixmap(),
							const QPixmap& pixRight=QPixmap(), const QPixmap& pixMiddle=QPixmap(),
							const QPixmap& pixSingle=QPixmap());

	//! Set the brush color for all the sub elements (Seems not to do anything, in fact!)
	void setColor(const QColor& c);

	//! Set whether time must be displayed in the bottom bar
	void setFlagShowTime(bool b) {flagShowTime=b;}
	bool getFlagShowTime() const { return flagShowTime; }
	//! Set whether location info must be displayed in the bottom bar
	void setFlagShowLocation(bool b) {flagShowLocation=b;}
	bool getFlagShowLocation() const { return flagShowLocation; }
	//! Set whether FPS info must be displayed in the bottom bar
	void setFlagShowFps(bool b) {flagShowFps=b;}
	bool getFlagShowFps() const { return flagShowFps; }
	//! Set whether FOV info must be displayed in the bottom bar
	void setFlagShowFov(bool b) {flagShowFov=b;}
	bool getFlagShowFov() const { return flagShowFov; }
	//! Set whether DMS format for FOV info must be displayed in the bottom bar
	void setFlagFovDms(bool b) {flagFovDms=b;}
	bool getFlagFovDms() const { return flagFovDms; }
	//! Set whether JD for time info must be displayed in the bottom bar
	void setFlagTimeJd(bool b) {flagTimeJd=b;}
	bool getFlagTimeJd() const { return flagTimeJd; }

	void setFlagShowTz(bool b) { flagShowTZ=b; }
	bool getFlagShowTz() const { return flagShowTZ; }

	//! @return boundingRect of the buttons only
	QRectF getButtonsBoundingRect() const;
	//! @return height of vertical gap (pixels)
	int getGap() const {return gap;}

	//! enable connection to StelCore::flagUseTopocentricCoordinatesChanged()
	//! Recommended in operations that temporarily switch off and back on.
	void enableTopoCentricUpdate(bool enable);
signals:
	void sizeChanged();

private slots:
	//! Update the help label when a button is hovered
	void buttonHoverChanged(bool b);

	//! connect from StelApp to resize fonts on the fly.
	void setFontSizeFromApp(int size);
	//! connect from StelApp to set font on the fly.
	void setFont(const QFont &cfont);

private:
	// updateTopocentric: regardless of topocentric setting, reformat the string if true
	void updateText(bool forceUpdatePos=false, bool updateTopocentric=false);
	void updateButtonsGroups();
	// Elements which get displayed above the buttons:
	QGraphicsSimpleTextItem* location;
	QGraphicsSimpleTextItem* datetime;
	QGraphicsSimpleTextItem* fov;
	QGraphicsSimpleTextItem* fps;
	int gap; // a pixel distance between status line and buttons. May have fixed size or could depend on status element font size QFontMetrics::descent()



	struct ButtonGroup
	{
		ButtonGroup() : leftMargin(0), rightMargin(0),
						pixBackgroundLeft(nullptr), pixBackgroundRight(nullptr),
						pixBackgroundMiddle(nullptr), pixBackgroundSingle(nullptr) {}
		//! Elements of the group
		QList<StelButton*> elems;
		//! Left margin size in pixel
		int leftMargin;
		//! Right margin size in pixel
		int rightMargin;
		//! Background Images;
		QPixmap* pixBackgroundLeft;
		QPixmap* pixBackgroundRight;
		QPixmap* pixBackgroundMiddle;
		QPixmap* pixBackgroundSingle;
	};

	QMap<QString, ButtonGroup> buttonGroups;
	QPixmap pixBackgroundLeft;
	QPixmap pixBackgroundRight;
	QPixmap pixBackgroundMiddle;
	QPixmap pixBackgroundSingle;

	bool flagShowTime;
	bool flagShowLocation;
	bool flagShowFps;
	bool flagShowFov;
	bool flagFovDms;
	bool flagTimeJd;
	bool flagShowTZ;
	// Store these names. They can potentially be changed each frame, but lookup from SolarSystem is very costly.
	QString planetNameEnglish;
	QString planetNameI18n;

	QGraphicsSimpleTextItem* helpLabel;
};

//! @class StelBarsFrame
//! @brief The path around the bottom and left button bars
class StelBarsFrame : public QGraphicsPathItem
{
	public:
		StelBarsFrame(QGraphicsItem* parent);
		//! defines a line around the two button bars
		void updatePath(BottomStelBar* bottom, LeftStelBar* left);
		//! return radius of corner arc
		double getRoundSize() const {return roundSize;}
		void setBackgroundOpacity(double opacity);
	private:
		double roundSize; // radius of corner arc
};

#endif // STELGUIITEMS_HPP
