import java.awt.*;

/********************************************************************************
** This canvas derived class is used to display an icon image.
*/

public class Icon extends Canvas
{
	/********************************************************************************
	** Constructor.
	*/

	/********************************************************************************
	** Set the image to display.
	*/

	public void setImage(Image oImage)
	{
		m_imgIcon         = oImage;
		m_rcBounds.x      = 0;
		m_rcBounds.y      = 0;
		m_rcBounds.width  = m_imgIcon.getWidth(null);
		m_rcBounds.height = m_imgIcon.getHeight(null);
	}

	public void setImage(Image oImage, Rectangle rcBounds)
	{
		m_imgIcon  = oImage;
		m_rcBounds = rcBounds.getBounds();
	}

	/********************************************************************************
	** Get the preferred size.
	*/

	public Dimension getPreferredSize()
	{
		return m_rcBounds.getSize();
	}

	/********************************************************************************
	** Paint the control.
	*/

	public void paint(Graphics g)
	{
		Dimension dmSize = getSize();

		g.setColor(SystemColor.controlShadow);
		g.fillRect(0, 0, dmSize.width, dmSize.height);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Image		m_imgIcon;
	private Rectangle	m_rcBounds = new Rectangle(0, 0, 32, 32);
}
