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
	public int     getAngle()		{ return m_nAngle;      }
	public boolean getPenDown()		{ return m_bPenDown;    }
	public String  getPenColour()	{ return m_strPenClr;   }
	public boolean isVisible()		{ return m_bShowTurtle; }

	/********************************************************************************
	** Reset the turtles state.
	*/

	public void reset()
	{
		Dimension dmDisplay = m_oDisplay.getDimensions();

		// Move to home position (centre).
		rotate(0);
		move(dmDisplay.width / 2, dmDisplay.height / 2);
		m_bPenDown  = true;
		m_clrPen    = Color.black;
		m_strPenClr = "Black";

		// Clear display.
		m_oDisplay.clear();
		drawTurtle();

		m_oDisplay.repaint();
	}

	/********************************************************************************
	** Show or hide the turtle.
	*/

	public void showTurtle(boolean bShow)
	{
		// Ignore, if no change.
		if (m_bShowTurtle == bShow)
			return;

		if (m_bShowTurtle)
			drawTurtle();

		m_bShowTurtle = bShow;

		if (m_bShowTurtle)
			drawTurtle();

		m_oDisplay.repaint();
		notifyListeners();
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

		notifyListeners();
	}

	/********************************************************************************
	** Set the pen colour.
	*/

	public void setPenColour(Color clr, String str)
	{
		m_clrPen    = clr;
		m_strPenClr = str;

		notifyListeners();
	}

	/********************************************************************************
	** Move the turtle forward by the given number of pixels.
	*/

	public void forward(int nPixels)
	{
		double dRad = m_nAngle * (Math.PI / 180.0);

		// Calculate the turtles destination.
		int dx = (int) Math.round(m_ptOrigin.x + (nPixels * Math.sin(dRad)));
		int dy = (int) Math.round(m_ptOrigin.y - (nPixels * Math.cos(dRad)));

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

	public void turnRight(int nDegrees)
	{
		if (m_bShowTurtle)
			drawTurtle();

		rotate(m_nAngle + nDegrees);

		if (m_bShowTurtle)
			drawTurtle();

		m_oDisplay.repaint();
	}

	/********************************************************************************
	** Turn the turtle left by the given number of degrees.
	*/

	public void turnLeft(int nDegrees)
	{
		if (m_bShowTurtle)
			drawTurtle();

		rotate(m_nAngle - nDegrees);

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
		notifyListeners();
	}

	/********************************************************************************
	** Rotate the turtle to the new angle.
	*/

	private void rotate(int nDegrees)
	{
		m_nAngle = (nDegrees + 360) % 360;

		calcShape();
		notifyListeners();
	}

	/********************************************************************************
	** Calculate the turtle shape.
	*/

	private void calcShape()
	{
		for (int i = 0; i < m_aptTurtle.length; i++)
		{
			double dRad = ((ANGLES[i] + m_nAngle) % 360) * (Math.PI / 180.0);

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

	private void notifyListeners()
	{
		if (m_oListeners != null)
			m_oListeners.actionPerformed(new ActionEvent(this, ActionEvent.ACTION_PERFORMED, ""));
	}

	/********************************************************************************
	** Constants
	*/

	// Turtle shape as points relative to origin.
	private static final double	RADIUS = 10.0;
	private static final int[]	ANGLES = new int[] { 0, 135, 225, 0 };

	/********************************************************************************
	** Members.
	*/

	// Turtle state.
	private boolean	m_bShowTurtle = true;
	private Color	m_clrTurtle   = Color.red;
	private Point	m_ptOrigin    = new Point(0, 0);
	private	int		m_nAngle      = 0;
	private Point[]	m_aptTurtle   = new Point[ANGLES.length];
	private boolean	m_bPenDown    = true;
	private Color	m_clrPen      = Color.black;
	private String	m_strPenClr   = "Black";

	// Output device.
	private Display	m_oDisplay;

	// Event listeners.
	private ActionListener m_oListeners;
}
