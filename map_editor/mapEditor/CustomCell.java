package mapEditor;

import javax.swing.*;

import java.awt.*;


public class CustomCell extends JLabel
{
	private int row;
	private int col;
		
	public CustomCell(int row, int col, ImageIcon pic)
	{
		new JLabel(pic);
		this.row = row;
		this.col = col;
		setBorder(BorderFactory.createLineBorder(Color.blue, 2));		
	}
	
	public CustomCell(int row, int col, String txt)
	{
		new JLabel(txt);
		this.row = row;
		this.col = col;
	}
	
	public void changeCell(ImageIcon pic)
	{
		setIcon(pic);		
	}
	
	public int getRow() { return row; }
	public int getCol() { return col; }
	public JLabel getLabel() { return this; }
}
