import java.awt.*;
import java.awt.event.*;

/********************************************************************************
** This is the turtle controlled by the program.
*/

public class Turtle
{
	/********************************************************************************
	** Constructor.
	*/

	public Turtle(Display oDisplay)
	{
		// Save parameters.
		m_oDisplay = oDisplay;

		// Allocate turtle shape points.
		for (int i = 0; i < ANGLES.length; i++)
			m_aptTurtle[i] = new Point();
	}

	/********************************************************************************
	** State accessors.
	*/

	public int     getXPos()		{ return m_ptOrigin.x;  }
	public int     getYPos()		{ return m_ptOrigin.y;  }
	public double  getAngle()		{ return m_dAngle;      }
	public boolean getPenDown()		{ return m_bPenDown;    }
	public String  getPenColour()	{ return m_strPenClr;   }
	public boolean isVisible()		{ return m_bShowTurtle; }

	/********************************************************************************
	** Reset the turtles state.
	*/

	public void reset(boolean bShow)
	{
		Dimension dmDisplay = m_oDisplay.getDimensions();

		showTurtle(false);

		// Clear display.
		m_oDisplay.clear();

		// Reset state.
		rotate(0);
		move(dmDisplay.width / 2, dmDisplay.height / 2);
		setPenDown(true);
		setPenColour(Color.black, "Black");
		showTurtle(bShow);

		m_oDisplay.repaint();
	}

	/********************************************************************************
	** Show or hide the turtle.
	*/

	public void showTurtle(boolean bShow)
	{
		if (m_bShowTurtle)
			drawTurtle();

		m_bShowTurtle = bShow;

		if (m_bShowTurtle)
			drawTurtle();

		m_oDisplay.repaint();
		notifyListeners(VISIBILITY);
	}

	/********************************************************************************
	** Set the turtles' colour.
	*/

	public void setTurtleColour(Color clr)
	{
		if (m_bShowTurtle)
			drawTurtle();

		m_clrTurtle = clr;

		if (m_bShowTurtle)
			drawTurtle();

		m_oDisplay.repaint();
	}

	/********************************************************************************
	** Set the pen state.
	*/

	public void setPenDown(boolean bDown)
	{
		m_bPenDown = bDown;

		notifyListeners(PENSTATE);
	}

	/********************************************************************************
	** Set the pen colour.
	*/

	public void setPenColour(Color clr, String str)
	{
		m_clrPen    = clr;
		m_strPenClr = str;

		notifyListeners(PENCOLOR);
	}

	/********************************************************************************
	** Move the turtle forward by the given number of pixels.
	*/

	public void forward(double dPixels)
	{
		double dRad = m_dAngle * (Math.PI / 180.0);

		// Calculate the turtles destination.
		int dx = (int) Math.round(m_ptOrigin.x + (dPixels * Math.sin(dRad)));
		int dy = (int) Math.round(m_ptOrigin.y - (dPixels * Math.cos(dRad)));

		if (m_bShowTurtle)
			drawTurtle();

		// If pen down, draw a line.
		if (m_bPenDown)
		{
			m_oDisplay.setPaintMode(false, null);
			m_oDisplay.setLineColour(m_clrPen);
			m_oDisplay.drawLine(m_ptOrigin.x, m_ptOrigin.y, dx, dy);
		}

		// Update the turtles pos.
		move(dx, dy);

		if (m_bShowTurtle)
			drawTurtle();

		m_oDisplay.repaint();
	}

	/********************************************************************************
	** Turn the turtle right by the given number of degrees.
	*/

	public void turnRight(double dDegrees)
	{
		if (m_bShowTurtle)
			drawTurtle();

		rotate(m_dAngle + dDegrees);

		if (m_bShowTurtle)
			drawTurtle();

		m_oDisplay.repaint();
	}

	/********************************************************************************
	** Turn the turtle left by the given number of degrees.
	*/

	public void turnLeft(double dDegrees)
	{
		if (m_bShowTurtle)
			drawTurtle();

		rotate(m_dAngle - dDegrees);

		if (m_bShowTurtle)
			drawTurtle();

		m_oDisplay.repaint();
	}

	/********************************************************************************
	** Event handler methods.
	*/

	public void addActionListener(ActionListener l)
	{
		m_oListeners = AWTEventMulticaster.add(m_oListeners, l);
	}

	public void removeActionListener(ActionListener l)
	{
		m_oListeners = AWTEventMulticaster.remove(m_oListeners, l);
	}

	/********************************************************************************
	** Move the turtle to a new position.
	*/

	private void move(int x, int y)
	{
		// Update the turtles' origin.
		m_ptOrigin.x = x;
		m_ptOrigin.y = y;

		calcShape();
		notifyListeners(MOVED);
	}

	/********************************************************************************
	** Rotate the turtle to the new angle.
	*/

	private void rotate(double dDegrees)
	{
		m_dAngle = (dDegrees + 360) % 360;

		calcShape();
		notifyListeners(ROTATED);
	}

	/********************************************************************************
	** Calculate the turtle shape.
	*/

	private void calcShape()
	{
		for (int i = 0; i < m_aptTurtle.length; i++)
		{
			double dRad = ((ANGLES[i] + m_dAngle) % 360) * (Math.PI / 180.0);

			// Calculate the point.
			m_aptTurtle[i].x = (int) (m_ptOrigin.x + (RADIUS * Math.sin(dRad)));
			m_aptTurtle[i].y = (int) (m_ptOrigin.y - (RADIUS * Math.cos(dRad)));
		}
	}

	/********************************************************************************
	** Draw or undraw the turtle.
	*/

	private void drawTurtle()
	{
		m_oDisplay.setPaintMode(true, Color.white);
		m_oDisplay.setLineColour(m_clrTurtle);
		m_oDisplay.drawPolyline(m_aptTurtle);
	}

	/********************************************************************************
	** Notify event listeners of the status change.
	*/

	private void notifyListeners(String strEvent)
	{
		if (m_oListeners != null)
			m_oListeners.actionPerformed(new ActionEvent(this, ActionEvent.ACTION_PERFORMED, strEvent));
	}

	/********************************************************************************
	** Constants
	*/

	// Turtle shape as points relative to origin.
	private static final double	RADIUS = 10.0;
	private static final int[]	ANGLES = new int[] { 0, 135, 225, 0 };

	// Turtle events.
	public static final String VISIBILITY = "Visibility";
	public static final String PENSTATE   = "PenState";
	public static final String PENCOLOR   = "PenColor";
	public static final String MOVED      = "Moved";
	public static final String ROTATED    = "Rotated";

	/********************************************************************************
	** Members.
	*/

	// Turtle state.
	private boolean	m_bShowTurtle = true;
	private Color	m_clrTurtle   = Color.red;
	private Point	m_ptOrigin    = new Point(0, 0);
	private	double	m_dAngle      = 0.0;
	private Point[]	m_aptTurtle   = new Point[ANGLES.length];
	private boolean	m_bPenDown    = true;
	private Color	m_clrPen      = Color.black;
	private String	m_strPenClr   = "Black";

	// Output device.
	private Display	m_oDisplay;

	// Event listeners.
	private ActionListener m_oListeners;
}
