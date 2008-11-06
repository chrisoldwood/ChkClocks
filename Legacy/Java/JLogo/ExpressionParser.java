/********************************************************************************
** This class is used to validate and evaluate expressions.
*/

public class ExpressionParser
{
	/********************************************************************************
	** Constructor.
	*/

	public ExpressionParser(Variables oVars)
	{
		m_oVars = oVars;
	}

	/********************************************************************************
	** Process the expression ready for evaluating.
	*/

	public double evaluate(String strExpr) throws ExpressionException
	{
		try
		{
			int			 nStrLen = strExpr.length();
			int          nLevel  = 0;
			StringBuffer str     = new StringBuffer(nStrLen);

			// Strip whitespace and check parenthesis matching.
			for (int i = 0; i < nStrLen; i++)
			{
				char cChar = strExpr.charAt(i);

				// Ignore char, if whitespace.
				if ( (cChar == ' ' ) || (cChar == '\t')
				  || (cChar == '\r') || (cChar == '\n') )
					continue;

				str.append(cChar);

				// If parenthesis, update nesting level.
				if (cChar == '(')	nLevel++;
				if (cChar == ')')	nLevel--;

				// Nesting level invalid?
				if (nLevel < 0)
					throw new ExpressionException("Mismatched parenthesis");
			}

			// Nesting level invalid?
			if (nLevel != 0)
				throw new ExpressionException("Mismatched parenthesis");

			return doEvaluation(str.toString());
		}
		catch(ExpressionException e)
		{
			throw new ExpressionException(e.getMessage() + " in expression:\n\n" + strExpr);
		}
	}

	/********************************************************************************
	** Evaluates the expression.
	*/

	private double doEvaluation(String strExpr) throws ExpressionException
	{
		int		nStrLen   = strExpr.length();
		int		nChar     = 0;
		double	dValue    = 0.0;
		char	cOperator = '+';

		if (nStrLen == 0)
			throw new ExpressionException("Empty subexpression");

		// Until evaluated.
		while (nChar < nStrLen)
		{
			// Not first char?
			if (nChar > 0)
			{
				// Extract operator.
				cOperator = strExpr.charAt(nChar++);

				// Valid.
				if (OPERATORS.indexOf(cOperator) < 0)
					throw new ExpressionException("Missing operator");
			}

			// Have a rhs value?
			if (nChar >= nStrLen)
				throw new ExpressionException("Missing r.h.s value");

			// Extract the sub-expression, constant or variable.
			char cChar = strExpr.charAt(nChar);

			// Is start of sub-expression?
			if (cChar == '(')
			{
				String strSubExpr = extractSubExpression(strExpr, nChar, nStrLen);

				// Empty sub-expression?
				if (strSubExpr.length() == 0)
					throw new ExpressionException("Empty parenthesis");

				dValue  = calcValue(dValue, cOperator, doEvaluation(strSubExpr));
				nChar  += strSubExpr.length() + 2;
			}
			// Is a constant?
			else if ( (cChar >= '0') && (cChar <= '9') )
			{
				String strConstant = extractConstant(strExpr, nChar, nStrLen);

				try
				{
					dValue = calcValue(dValue, cOperator, Double.valueOf(strConstant).doubleValue());
					nChar += strConstant.length();
				}
				catch(NumberFormatException e)
				{
					throw new ExpressionException("Invalid constant " + strConstant);
				}
			}
			// Is a variable.
			else 
			{
				String strVariable = extractVariable(strExpr, nChar, nStrLen);

				if (!m_oVars.exists(strVariable))
					throw new ExpressionException("Invalid variable " + strVariable);

				dValue  = calcValue(dValue, cOperator, m_oVars.get(strVariable));
				nChar  += strVariable.length();
			}
		}

		return dValue;
	}

	/********************************************************************************
	** Extract a sub-expression from the expression.
	*/

	private String extractSubExpression(String strExpr, int nStart, int nEnd)
	{
		int nLevel = 1;

		// Find matching bracket.
		for (int i = nStart+1; i < nEnd-1; i++)
		{
			char cChar = strExpr.charAt(i);

			// If bracket, update nesting level.
			if (cChar == '(')	nLevel++;
			if (cChar == ')')	nLevel--;

			// Found mathing bracket?
			if (nLevel == 0)
			{
				return strExpr.substring(nStart+1, i);
			}
		}

		// Whole string is sub-expression.
		return strExpr.substring(nStart+1, nEnd-1);
	}

	/********************************************************************************
	** Extract a constant from the expression.
	*/

	private String extractConstant(String strExpr, int nStart, int nEnd)
	{
		// Find end of constant.
		for (int i = nStart; i < nEnd; i++)
		{
			char cChar = strExpr.charAt(i);

			// Found end of constant?
			if ( ((cChar < '0') || (cChar > '9')) && (cChar != '.') )
			{
				return strExpr.substring(nStart, i);
			}
		}

		// Whole string is a constant.
		return strExpr.substring(nStart, nEnd);
	}

	/********************************************************************************
	** Extract a variable from the expression.
	*/

	private String extractVariable(String strExpr, int nStart, int nEnd)
	{
		// Find end of variable.
		for (int i = nStart; i < nEnd; i++)
		{
			char cChar = strExpr.charAt(i);

			// Found end of variable?
			if (OPERATORS.indexOf(cChar) > 0)
			{
				return strExpr.substring(nStart, i);
			}
		}

		// Whole string is a variable.
		return strExpr.substring(nStart, nEnd);
	}

	/********************************************************************************
	** Apply the operator to the left and right hande sides.
	*/

	private double calcValue(double dLHS, char cOperator, double dRHS)
	{
		if ( (cOperator == '/') && (dRHS == 0.0) )
			throw new ExpressionException("Division by zero");

		switch (cOperator)
		{
			case '+':	return dLHS + dRHS;
			case '-':	return dLHS - dRHS;
			case '*':	return dLHS * dRHS;
			case '/':	return dLHS / dRHS;
		}

		return 0.0;
	}

	/********************************************************************************
	** Constants.
	*/

	// Supported operators.
	public static final String OPERATORS = "+-*/";

	/********************************************************************************
	** Members.
	*/

	private Variables	m_oVars;
}
