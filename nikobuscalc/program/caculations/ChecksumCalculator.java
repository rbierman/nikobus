package program.caculations;

public class ChecksumCalculator {
	private final static int START_VALUE = 0xFFFF;
	private final static int XOR_VALUE = 0x1021;
	
	private String adress;
	private String command;

	public ChecksumCalculator(String adress, String command) {
		this.adress = adress;
		this.command = command;
	}

	public String getFirstByte() {
		int commandAdressWord = Integer.parseInt(command,16);
		int firstAdressWord = Integer.parseInt(adress.substring(0, 2),16);
		int secondAdressWord = Integer.parseInt(adress.substring(2,4),16);
		
		
		long result = 0;
		result = this.calculateCRC(START_VALUE, commandAdressWord);
		result = this.calculateCRC(result,secondAdressWord);
		result = this.calculateCRC(result,firstAdressWord);
		
		//Trim first char's the are not needed!
		result = result&0xffff;
		return "" + Long.toHexString(result).substring(0, 2);
	}

	public String getSecondByte() {
		int commandAdressWord = Integer.parseInt(command,16);
		int firstAdressWord = Integer.parseInt(adress.substring(0, 2),16);
		int secondAdressWord = Integer.parseInt(adress.substring(2,4),16);
		
		long result = 0;
		result = this.calculateCRC(START_VALUE, commandAdressWord);
		result = this.calculateCRC(result,secondAdressWord);
		result = this.calculateCRC(result,firstAdressWord);
		
		//Trim first char's the are not needed!
		result = result&0xffff;
		return "" + Long.toHexString(result).substring(2, 4);
	}

	public String getThirdByte() {
		return null;
	}
	
	
	private long calculateCRC(long startWord, long messageWord) {
		long result = startWord;
		
		//Shift messageword to the high position.
		result = messageWord << 8;
		
		//XOR result with the shifted result
		result = startWord^result;
		
		for(int i=0; i<8;i++)
		{
			//Check if we need to XOR this later on!
			boolean XORLater = false;
			if((result&0xFFFF) >= 0x8000)
			{
				XORLater = true;
			}
			
			//Multiply result by 2
			result = result * 2;
			
			if(XORLater)
			{
				result = result ^ XOR_VALUE;
			}
		
		}
		
		
		return result;
	}

}
