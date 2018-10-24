/* This code adds all the values of an array */

public class MainSerialPi {
	private static long NUM_RECTS = 100_000_000;
	
	public static void main(String args[]) {
		long startTime, stopTime;
		double width, area, acum = 0;
		
		width = 1.0 / (double) NUM_RECTS;
		SerialPi e = new SerialPi(NUM_RECTS, width);
		
		acum = 0;
		for (int i = 0; i < Utils.N; i++) {
			startTime = System.currentTimeMillis();
			e.calculate();
			stopTime = System.currentTimeMillis();
			acum += (stopTime - startTime);
		}
		
		area = width * e.getSum();
		System.out.printf("sum = %.15f\n", area);
		System.out.printf("avg time = %.5f\n", (acum / Utils.N));
	}
}
