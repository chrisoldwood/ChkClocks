import java.awt.*;

/********************************************************************************
** This exception is thrown when the user has supplied an invalid parameter.
*/

public class InvalidParamException extends Exception
{
	/********************************************************************************
	** Constructor.
	*/

	public InvalidParamException(String strMsg, Component oField)
	{
		super(strMsg);

		m_oField = oField;
	}

	/********************************************************************************
	** Sets the focus to the control with the error.
	*/

	public void setFocus()
	{
		if (m_oField != null)
			m_oField.requestFocus();
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Component	m_oField;
}
