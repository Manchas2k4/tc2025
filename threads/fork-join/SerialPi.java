/* This code adds all the values of an array */

public class SerialPi {
	private long size;
	private double sum, width;
	
	public SerialPi(long size, double width) {
		this.size = size;
		this.width = width;
		this.sum = 0;
	}
	
	public double getSum() {
		return sum;
	}
	
	public void calculate() {
		double mid, height;
		
		sum = 0;
		for (int i = 0; i < size; i++) {
			mid = (i + 0.5) * width;
			height = 4.0 / (1.0 + (mid * mid));
			sum += height;
		}
	}
}
			
