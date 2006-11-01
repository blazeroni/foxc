package mapEditor;

import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.io.*;

public class MainMenu extends JFrame implements ActionListener
{
	private static final long serialVersionUID = 1L;
	JButton btExit = new JButton("Exit");
	JButton btKillBorder = new JButton("Kill Borders");
	JButton btSaveMap = new JButton("Save Map");
	JButton btLoadMap = new JButton("Load Map");
	JButton btGenerate = new JButton("Generate");
	private JScrollBar sbBar;
	private JLabel[] columns;
	private JLabel[] row;
	MapTable newTable;

	public MainMenu(String title, Integer cols, Integer rows, Integer tileSize)
	{
	    super(title);
		setLocation(30, 30);
		setSize(900, 750);
		Container container = getContentPane();
		setLayout(null);
		
		//ask for size
		newTable = new MapTable(rows, cols, tileSize);
		columns = new JLabel[rows];
		row = new JLabel[cols];
		container.add(newTable);
		btExit.setBounds(0, 0, 150, 50);
		btExit.setLocation(20, 20);
		btKillBorder.setBounds(0, 0, 150, 50);
		btKillBorder.setLocation(190, 20);
		btSaveMap.setBounds(0, 0, 150, 50);
		btSaveMap.setLocation(360, 20);
		btLoadMap.setBounds(0, 0, 150, 50);
		btLoadMap.setLocation(530, 20);
		btGenerate.setBounds(0, 0, 150, 50);
		btGenerate.setLocation(700, 20);
		container.add(btExit);		
		container.add(btKillBorder);
		container.add(btLoadMap);	
		container.add(btSaveMap);
		container.add(btGenerate);	
		btExit.addActionListener(this);
		btKillBorder.addActionListener(this);
		btSaveMap.addActionListener(this);
		btLoadMap.addActionListener(this);
		btGenerate.addActionListener(this);
		sbBar = new JScrollBar();
		add(sbBar);
		
		//Container container = getContentPane();
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setVisible(true);
		
		for(Integer i = 0; i < rows; i++)
        {
        	columns[i] = new JLabel(i.toString());
        	columns[i].setLayout(null);
    		columns[i].setSize(tileSize,tileSize);
        	columns[i].setLocation(108+tileSize*i, 70);
        	add(columns[i]);
        }
        
        for(Integer i = 0; i < cols; i++)
        {
        	row[i] = new JLabel(i.toString());
        	row[i].setLayout(null);
    		row[i].setSize(tileSize,tileSize);
        	row[i].setLocation(70, 108+tileSize*i);
        	add(row[i]);
        }
		
	}	
	
	public static void main(String args[])
	{
		Integer mainRow = Integer.parseInt(JOptionPane.showInputDialog(null, "enter the number of rows:", "X-CLONE", JOptionPane.QUESTION_MESSAGE));
	    Integer mainCol = Integer.parseInt(JOptionPane.showInputDialog(null, "enter the number of columns:", "X-CLONE", JOptionPane.QUESTION_MESSAGE));
	    Integer tiles = Integer.parseInt(JOptionPane.showInputDialog(null, "enter the size of the tiles:", "X-CLONE", JOptionPane.QUESTION_MESSAGE));
		MainMenu X = new MainMenu("MAP GENERATOR", mainRow, mainCol, tiles);
	}

	public void actionPerformed(ActionEvent e) 
	{	
		if(e.getSource() == btExit)
		{
			System.exit(0);
		}
		if(e.getSource() == btKillBorder)
		{
			newTable.killBorders();
			if(e.getActionCommand().equals("Kill Borders"))
			{
				btKillBorder.setText("Restore Borders");
			}
			else
			{
				btKillBorder.setText("Kill Borders");
			}			
		}
		if(e.getSource() == btSaveMap)
		{
			PrintWriter writer = null;
		    try
	        {
		    	String Name = JOptionPane.showInputDialog(null, "enter The FileName", "X-CLONE", JOptionPane.QUESTION_MESSAGE);
	            writer = new PrintWriter(Name);
	        }		    
	        catch(IOException ee) {}
	        JLabel Temp = null;
	        String cmd = null;
	        for(int i = 0; i < row.length; i++)
	        {
	        	for(int v = 0; v < columns.length; v++)
		        {
	        		Temp = newTable.getData(i, v);
	        		if((Temp.getIcon()) != null)
	        		{
	        			cmd =  new String((Temp.getIcon()).toString());
	        			writer.write(cmd, 0 , cmd.length());
	        			writer.println();                
	        		}
	        		else
	        		{
	        			cmd =  new String("noPic");
	        			writer.write(cmd, 0 , cmd.length());
	        			writer.println();                
	        		}
	        		writer.flush();
		        }		        
	        }
	        writer.close();
	        JOptionPane.showMessageDialog(null, "Map Saved!", "X-CLONE!", JOptionPane.WARNING_MESSAGE);
		}
		
		if(e.getSource() == btGenerate)
		{
			PrintWriter writer = null;
		    try
	        {
		    	String Name = JOptionPane.showInputDialog(null, "enter The FileName", "X-CLONE", JOptionPane.QUESTION_MESSAGE);
		    	Name = new String(Name + ".xcm");
	            writer = new PrintWriter(Name);
	        }		    
	        catch(IOException ee) {}
	        JLabel Temp = null;
	        String cmd = null;
	        
	        cmd = new String("<map name=\"test map\" width=" + "\"" + Integer.toString(row.length) + "\" height=" + "\"" + Integer.toString(columns.length) + "\">\n"
	        		         + "      <tiles>");	        		         
	        writer.write(cmd, 0 , cmd.length());
			writer.println();
			writer.flush();
			
			cmd = null;
			Integer Id = 1;
	        for(int i = 0; i < row.length; i++)
	        {
	        	for(int v = 0; v < columns.length; v++)
		        {
	        		Temp = newTable.getData(v, i);
	        		if((Temp.getIcon()) != null)
	        		{
	        			cmd =  new String((Temp.getIcon()).toString());
	        			cmd = cmd.substring(cmd.length() - 9);
	        			if(cmd.equalsIgnoreCase("grass.PNG"))
	        			    cmd = new String("     <tile id=\"" + (Id++) + "\" x=\"" + i + "\" y= \"" + v + "\" terrain=\"grass\"/>");
	        							    //<tile id= "      1       "  x= "    0   "   y=   "   0     " terrain= "grass"/>
	        			else if(cmd.equalsIgnoreCase("water.PNG"))
	        				cmd = new String("     <tile id=\"" + (Id++) + "\" x=\"" + i + "\" y= \"" + v + "\" terrain=\"water\"/>");
	        			
	        			writer.write(cmd, 0 , cmd.length());
	        			writer.println();                
	        		}
	        		else
	        		{
	        			cmd =  new String("noPic");
	        			writer.write(cmd, 0 , cmd.length());
	        			writer.println();                
	        		}
	        		writer.flush();
		        }		        
	        }
	        cmd = new String("   </tiles>\n" +
	        		         "   <edges>\n" +
	        		         // THIS LINE MUST GENERATE THE WALLS WITH WALLS
	        		         "      <edge type=\"wall\" id1=\"1\" id2=\"2\"/>" +
	        		         "   </edges>\n" +
	        		         "</map>");	        	        
	        writer.write(cmd, 0 , cmd.length());
	        writer.println();
	        writer.flush();
	
	        writer.close();
	        JOptionPane.showMessageDialog(null, "Map Generated!", "X-CLONE!", JOptionPane.WARNING_MESSAGE);
		}
		
		if(e.getSource() == btLoadMap)
		{
			String Name = null;
			FileInputStream where;
	        try 
		    {	        	
	        	while(Name == null)
	        	{
	        	    Name = JOptionPane.showInputDialog(null, "enter The MapName", "X-CLONE", JOptionPane.QUESTION_MESSAGE);
	        	}
		        where = new FileInputStream(Name);
		    } catch(FileNotFoundException x) {
		        System.out.print("File Not Found!, Contact Your System Administrator");
	            return;
	        }
		    InputStreamReader who = new InputStreamReader(where);
	        BufferedReader how = new BufferedReader(who);
	        
	        String User = "";
	        
	        while(User != null)
	        {
	        	try 
		        {
	        		for(int i = 0; i < row.length; i++)
			        {
			        	for(int v = 0; v < columns.length; v++)
				        {
			        	    User = how.readLine();
			        	    if(!User.equalsIgnoreCase("noPic") && User != null)
			        	    {
			        	    	newTable.setValueAt(i, v, new ImageIcon(User));
			        	    }
				        }
			        }
			    } catch (IOException e1) {
			       	e1.printStackTrace();
			    }
			    	        	
	        }
		}
	}
}
