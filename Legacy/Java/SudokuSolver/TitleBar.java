import java.awt.*;

/********************************************************************************
** This is the applications title bar, used to display status information.
*/

public class TitleBar extends Label
{
	/********************************************************************************
	** Constructor.
	*/

	public TitleBar()
	{
		super(SudokuSolver.APPLET_NAME, Label.CENTER);

		// Setup the default colours.
		setBackground(Color.blue);
		setForeground(Color.white);
	}

	/********************************************************************************
	** Sets the status information.
	*/

	public void setStatus(String strStatus)
	{
		StringBuffer strLabel = new StringBuffer(SudokuSolver.APPLET_NAME);

		if ( (strStatus != null) && (strStatus.length() > 0) )
		{
			strLabel.append(" [");
			strLabel.append(strStatus);
			strLabel.append("]");
		}

		setText(strLabel.toString());
	}

	/********************************************************************************
	** Constants
	*/

	/********************************************************************************
	** Members.
	*/
}
