package gui;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;

import program.caculations.ChecksumCalculator;


/**
* This code was edited or generated using CloudGarden's Jigloo
* SWT/Swing GUI Builder, which is free for non-commercial
* use. If Jigloo is being used commercially (ie, by a corporation,
* company or business for any purpose whatever) then you
* should purchase a license for each developer using Jigloo.
* Please visit www.cloudgarden.com for details.
* Use of Jigloo implies acceptance of these licensing terms.
* A COMMERCIAL LICENSE HAS NOT BEEN PURCHASED FOR
* THIS MACHINE, SO JIGLOO OR THIS CODE CANNOT BE USED
* LEGALLY FOR ANY CORPORATE OR COMMERCIAL PURPOSE.
*/
@SuppressWarnings("serial")
public class MainJFrame extends javax.swing.JFrame {
	private JLabel adressText;
	private JTextField commandField;
	private JPanel jPanel1;
	private JLabel CommandLabel;
	private JTextField adressField;

	private JButton calculateChecksum;
	private JLabel checksumByte3Text;
	private JLabel checksumByte2Text;
	private JLabel checksumByte1Text;

	/**
	* Auto-generated main method to display this JFrame
	*/
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				MainJFrame inst = new MainJFrame();
				inst.setLocationRelativeTo(null);
				inst.setVisible(true);
			}
		});
	}
	
	public MainJFrame() {
		super();
		initGUI();
	}
	
	private void initGUI() {
		try {
			GridBagLayout thisLayout = new GridBagLayout();
			thisLayout.rowWeights = new double[] {0.1, 0.1, 0.1, 0.1, 0.1};
			thisLayout.rowHeights = new int[] {3, 3, 3, 3, 3};
			thisLayout.columnWeights = new double[] {0.1};
			thisLayout.columnWidths = new int[] {7};
			getContentPane().setLayout(thisLayout);
			setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
			{
				adressText = new JLabel();
				getContentPane().add(adressText, new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				adressText.setText("Adres");
			}
			{
				adressField = new JTextField();
				getContentPane().add(adressField, new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				adressField.setPreferredSize(new java.awt.Dimension(80, 22));
				adressField.setText("2cb2");
			}
			{
				checksumByte1Text = new JLabel();
				getContentPane().add(checksumByte1Text, new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				checksumByte1Text.setText("First checksum:");
			}

			{
				checksumByte2Text = new JLabel();
				getContentPane().add(checksumByte2Text, new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				checksumByte2Text.setText("Second checksum:");
			}
			{
				checksumByte3Text = new JLabel();
				getContentPane().add(checksumByte3Text, new GridBagConstraints(0, 4, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				checksumByte3Text.setText("Third checksum:");
			}
			{
				calculateChecksum = new JButton();
				getContentPane().add(calculateChecksum, new GridBagConstraints(2, 1, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				calculateChecksum.setText("Do the MAGIC!");
				calculateChecksum.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent evt) {
						MainJFrame.this.calculateChecksum();
					}


				});
			}
			{
				CommandLabel = new JLabel();
				getContentPane().add(CommandLabel, new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				CommandLabel.setText("Command");
			}
			{
				jPanel1 = new JPanel();
				getContentPane().add(jPanel1, new GridBagConstraints(1, 3, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
			}
			{
				commandField = new JTextField();
				getContentPane().add(commandField, new GridBagConstraints(1, 1, 1, 1, 0.0, 0.0, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
				commandField.setText("17");
			}
			pack();
			setSize(400, 300);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void calculateChecksum() {
		ChecksumCalculator calculator = new ChecksumCalculator(this.adressField.getText(),this.commandField.getText());
		this.checksumByte1Text.setText("First checksum: " + calculator.getFirstByte());
		this.checksumByte2Text.setText("Second checksum: " + calculator.getSecondByte());
		
	}
}
