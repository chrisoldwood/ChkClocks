import java.awt.*;
import java.awt.event.*;
import java.util.*;

/********************************************************************************
** This class is used to store and manage the program commands.
*/

public class Program
	implements Runnable
{
	/********************************************************************************
	** Clear the program.
	*/

	public void clear()
	{
		m_oFirstCmd = null;
		m_oLastCmd  = null;
	}

	/********************************************************************************
	** Add the command to the program.
	*/

	public void add(Cmd oCmd)
	{
		// Push the command onto the stack.
		if (oCmd instanceof RepeatCmd)
		{
			m_vStack.push(oCmd);
			return;
		}

		// Loop being created?
		if (m_vStack.size() > 0)
		{
			RepeatCmd oRepeatCmd = (RepeatCmd) m_vStack.lastElement();

			// Add to the loop.
			oRepeatCmd.add(oCmd);

			// Not end of loop?
			if (!(oCmd instanceof EndRepeatCmd))
				return;

			EndRepeatCmd oEndCmd = (EndRepeatCmd) oCmd;

			// Pop the finished loop off the stack.
			m_vStack.pop();
			oEndCmd.setRepeatCmd(oRepeatCmd);
			oCmd = oRepeatCmd;

			// Not bottom of stack?
			if (m_vStack.size() > 0)
			{
				RepeatCmd oRepeatCmd2 = (RepeatCmd) m_vStack.lastElement();

				// Add to the loop.
				oRepeatCmd2.add(oCmd);

				return;
			}
		}

		if (m_oFirstCmd == null)
			m_oFirstCmd = oCmd;
		else
			m_oLastCmd.setNext(oCmd);

		m_oLastCmd = oCmd;
	}

	/********************************************************************************
	** Start the thread to execute the program.
	*/

	public void execute(Turtle oTurtle, int nInterval)
	{
		oTurtle.reset();

		m_oThread   = new Thread(this, "Execute");
		m_oTurtle   = oTurtle;
		m_nInterval = nInterval;

		m_oThread.start();
	}

	/********************************************************************************
	** Gets if the program is currently executing.
	*/

	public boolean isExecuting()
	{
		return (m_oThread != null);
	}

	/********************************************************************************
	** Execute the program, pausing between each command.
	*/

	public void run()
	{
		notifyListeners(START);

		for (Cmd oCmd = m_oFirstCmd; oCmd != null; oCmd = oCmd.getNext(true))
		{
			try
			{
				m_oThread.sleep(m_nInterval);
			}
			catch(InterruptedException e)
			{ }

			oCmd.execute(m_oTurtle);
		}

		notifyListeners(STOP);

		m_oThread = null;
		m_oTurtle = null;
	}

	/********************************************************************************
	** Convert the program to source code.
	*/

	public String getSource()
	{
		StringBuffer strSrc = new StringBuffer(256);
		int          nIndent = 0;

		for (Cmd oCmd = m_oFirstCmd; oCmd != null; oCmd = oCmd.getNext(false))
		{
			if (oCmd instanceof EndRepeatCmd)
				nIndent -=4 ;

			if (nIndent > 0)
			{
				for (int i = 0; i < nIndent; i++)
					strSrc.append(' ');
			}

			strSrc.append(oCmd.getSource());

			if (oCmd instanceof RepeatCmd)
				nIndent +=4 ;
		}

		return strSrc.toString();
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
	** Notify event listeners of the event.
	*/

	private void notifyListeners(String strEvent)
	{
		if (m_oListeners != null)
			m_oListeners.actionPerformed(new ActionEvent(this, ActionEvent.ACTION_PERFORMED, strEvent));
	}

	/********************************************************************************
	** Constants.
	*/

	// Event types.
	public static final String START = "START";
	public static final String STOP  = "STOP";

	/********************************************************************************
	** Members.
	*/

	// The command list.
	private Cmd		m_oFirstCmd;
	private Cmd		m_oLastCmd;
	private Stack	m_vStack = new Stack();

	// The execute thread.
	private Thread	m_oThread;
	private Turtle	m_oTurtle;
	private int		m_nInterval;

	// Event listeners.
	private ActionListener m_oListeners;
}
