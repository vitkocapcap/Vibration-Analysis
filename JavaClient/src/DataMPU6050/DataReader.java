package DataMPU6050;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.*;
import java.net.*;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Scanner;

import javax.swing.*;


public class DataReader extends JFrame implements ActionListener {
	public static final int SERVER_PORT= 80;
//public final static String SERVER_IP = "192.168.10.194";
	public static String SERVER_IP=null;
	public static boolean reconnectFlag=false;
	
	private JButton readData, close,setIp;
	private JPanel mainPanel, ipPanel;
	private JTextField serverIp;
	private JLabel status;
	
	
	public static BufferedReader br;
	public static PrintWriter os;
	
	ImageIcon imgread = new ImageIcon("resources/read.png");
	ImageIcon imgclose = new ImageIcon("resources/close.png");
	ImageIcon imgicon = new ImageIcon("resources/icon.png");
	
	public static Socket socket=null;
	
	public DataReader() {
		super("Data Reader");
		
		this.setIconImage(imgicon.getImage());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//Stop the program when exiting
		this.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent e){
                if(socket != null){
                    try {
                        socket.close();
                        System.exit(0);
                    } catch (IOException e1) {
                        System.out.println("Can not stop. Error: "+e1);
                    }
                }
			}	
		});
		//Start UI
		GUI(); 
	}
	
	
	private void GUI() {
		setSize(300, 200); 
		setResizable(false);
		setLocation(250,80);
		setLayout(new BorderLayout());
		
		//IP input
		serverIp = new JTextField(10);
		serverIp.setBackground(Color.WHITE);
		serverIp.setForeground(Color.RED);
		
		setIp = new JButton("SET");
		setIp.setBackground(Color.WHITE);
		setIp.addActionListener(this);
		
		
		ipPanel = new JPanel();
		ipPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
		ipPanel.add(serverIp);
		ipPanel.add(setIp);
		
		add(ipPanel,BorderLayout.NORTH);
		
		
		//Button
		readData= new JButton(imgread);
		readData.setBackground(Color.WHITE);
		readData.addActionListener(this); 
		
		close= new JButton(imgclose);
		close.setBackground(Color.WHITE);
		close.addActionListener(this); 
		
		//Order
		mainPanel=new JPanel();
		mainPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
		mainPanel.add(readData);
		mainPanel.add(close);
		
		
		add(mainPanel,BorderLayout.CENTER);
		
		//status
		status = new JLabel("Welcome to Data Reader. \u00a9 Viet&Giang",JLabel.CENTER);
		add(status,BorderLayout.SOUTH);
		
		setVisible(true);
	}
	

	private void accessServer() {	
		try {
			System.out.println("Connecting to server...");
			socket = new Socket(SERVER_IP, SERVER_PORT);
			System.out.println("Connected: " + socket);
			status.setText("Connected to: "+SERVER_IP);
			br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			os = new PrintWriter(socket.getOutputStream(), true);	
			System.out.println("Ready to go...");
		}
		catch (IOException ie){
			System.out.println("Can't connect to server: "+ie );
			JOptionPane.showMessageDialog(this,"Failed to connect to Server. Check your IP again","Error",JOptionPane.WARNING_MESSAGE);
			System.exit(0);
		}

	}
	

	public static void main(String[] args) throws InterruptedException {
		
		DataReader read = new DataReader();
		
		while(true) {
			System.out.print("");
			if(SERVER_IP!=null) {
				if(socket!=null) {
					continue;
				}
				else {
					System.out.println("SERVER...");
					read.accessServer();
				}
			}
			Thread.sleep(100);
		}
		
	}
	
	

	public void actionPerformed(ActionEvent e) {
		try {
	
		//If the user wants to log out of the chat box
		if(e.getSource()==close){
			System.out.println("Close pressed");
			if(socket != null) {
			try {
				socket.close();
				socket=null;
				SERVER_IP=null;
				System.out.println("*Connection has been released*");
				status.setText("You have disconnected to Server.");
				
				setIp.setEnabled(true);
				serverIp.setEditable(true);
			}
			catch(IOException closeerror) {
				System.out.println("Still connected.Error: "+closeerror);
				System.exit(1);
			}
			}
		}
		else if(e.getSource()==readData){
			if(socket!=null) {
			os.println("read");
			System.out.println("Client: Read.");
			System.out.println("Waiting for data from server...");
			status.setText("Waiting for data from server...");
			
			String readSize = br.readLine();
			
			int size = Integer.parseInt(readSize);
			System.out.println("Size: " + size);
			
			double arraydata2[] = new double[size];
			for(int i=0;i<size;i++) {
				String readArray = br.readLine();	
				arraydata2[i]=Double.parseDouble(readArray);;
			}
			
			
			System.out.println("Data has been received.");
			status.setText("Received data");
			//Time
			Calendar cal = Calendar.getInstance();
			String time= new SimpleDateFormat("dd.MMM.yy_HH.mm.ss").format(cal.getTime());
			
			//File
			File object = new File("runnable/Matlab_runnable.txt");
			object.delete();
			object.createNewFile();
				
			
			FileWriter runnable = new FileWriter("runnable/Matlab_runnable.txt");
			FileWriter backupObject = new FileWriter("Data/Data_"+time+ ".txt");	
			
			
			for(int i=0;i<size;i++) {
				
					System.out.println("Data "+ (i+1) +" : "+ arraydata2[i]);
					runnable.write(arraydata2[i]+ " ");
					backupObject.write(arraydata2[i]+ " ");
				
			}
			runnable.close();
			backupObject.close();
			}
			else
			{
				status.setText("Connect to server to receive data");
			}
		}
		else if(e.getSource()==setIp) {
			SERVER_IP=serverIp.getText();
			System.out.println(SERVER_IP);
			
			setIp.setText("Reconnect");
			setIp.setEnabled(false);
			serverIp.setEditable(false);
			
		}
		}
		catch(IOException e2){}
		
		
	}
	
}