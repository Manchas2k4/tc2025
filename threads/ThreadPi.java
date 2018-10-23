public class ThreadPi extends Thread {
	private int start, end;
	private double sum, width;
	
	public ThreadPi(double width, int start, int end) {
		this.width = width;
		this.start = start;
		this.end = end;
		this.sum = 0;
	}
	
	public double getSum() {
		return sum;
	}
	
	public void run() {
		double mid, height;
		
		sum = 0;
		for (int i = start; i < end; i++) {
			mid = (i + 0.5) * width;
			height = 4.0 / (1.0 + (mid * mid));
			sum += height;
		}
	}
}
			
