/********************************************************************************
** This exception is thrown by ExpressionParser when parsing an expression.
*/

public class ExpressionException extends RuntimeException
{
	/********************************************************************************
	** Constructor.
	*/

	public ExpressionException(String strMsg)
	{
		super(strMsg);
	}
}
