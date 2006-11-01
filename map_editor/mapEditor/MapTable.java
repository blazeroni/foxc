package mapEditor;

import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import javax.swing.ListSelectionModel;
import java.awt.Image;
import javax.swing.ImageIcon;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableColumn;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
import javax.swing.DefaultCellEditor;


public class MapTable extends JPanel implements MouseListener
{
	private JFileChooser fc;
	private JLabel[][] data;
	private JScrollBar sbBar;
	private int tileSize;
	
	public JLabel getData(int row, int col)
	{
		return data[row][col];
	}
	    	
    public MapTable(int row, int col, int tileSize) 
    {
    	super(null);
    	this.tileSize = tileSize;
    	setBounds(0,0,row*tileSize,col*tileSize);
    	setLocation(100, 100);
    	
    	data = new JLabel[row][col];
        
        for(Integer i = 0; i < row; i++)
        {
        	for(Integer v = 0; v < col; v++)
            {
        		data[i][v] = new JLabel("");
        		data[i][v].setBorder(BorderFactory.createLineBorder(Color.black, 1));
        		data[i][v].setLayout(null);
        		data[i][v].setSize(tileSize,tileSize);
        		data[i][v].setIcon(null);
        		// 				THIS CHOOSE THE SIZE OF THE TILES!
        		data[i][v].setLocation(tileSize*i, tileSize*v);
        		add(data[i][v]);
            }
        }
       
        fc = new JFileChooser();
	    addMouseListener(this);	    
    }
    
    public Object getValueAt(int row, int col) {
        return data[row][col];
    }
    
    public void setValueAt(int row, int col, ImageIcon ob1) 
    {
    	data[row][col].setIcon(ob1);
    }

	private void openFiles(int row, int col) 
	{
		int returnVal = fc.showOpenDialog(MapTable.this);
        if (returnVal == JFileChooser.APPROVE_OPTION) 
        {
        	File file = fc.getSelectedFile();
        	
	        data[row][col].setIcon(new ImageIcon(file.getPath()));
	    } 
        else 
	       System.out.print("Open command cancelled by user.\n");
	}	    
	
	/** Returns an ImageIcon, or null if the path was invalid. */
	//THIS METHOD SUCK ASS!
	protected static ImageIcon createImageIcon(String path) 
	{
		java.net.URL imgURL = MapTable.class.getResource(path);
		if (imgURL != null) 
		{
			return new ImageIcon(imgURL);
		} 
		else 
		{
			System.err.println("Couldn't find file: " + path);
			return null;
		}
	}

	public void mouseClicked(MouseEvent e) 
	{
		   //((JLabel)((MapTable)e.getComponent()).data[((int)e.getX()/tileSize)][((int)e.getY()/tileSize)]).setText("WORKS!");
		   
		   Integer col = ((int)e.getY()/tileSize);
		   Integer row = ((int)e.getX()/tileSize);
	       openFiles(row, col);
 	   		
	}
	
	public void killBorders()
	{
		if(data[0][0].getBorder() == null)
		{
			for(Integer i = 0; i < data.length; i++)
			{
				for(Integer v = 0; v < data[i].length; v++)
				{
					data[i][v].setBorder(BorderFactory.createLineBorder(Color.black, 1));
				}
			}
		}
		else
		{
			for(Integer i = 0; i < data.length; i++)
			{
				for(Integer v = 0; v < data[i].length; v++)
				{
					data[i][v].setBorder(null);
				}
			}
		}
	}

	public void mouseEntered(MouseEvent e) 
	{
		
	}

	public void mouseExited(MouseEvent e) 
	{
	    ((JComponent)(e.getSource())).setBackground(Color.orange);		
	}

	public void mousePressed(MouseEvent arg0) 
	{
		
	}

	public void mouseReleased(MouseEvent arg0) 
	{}
}
