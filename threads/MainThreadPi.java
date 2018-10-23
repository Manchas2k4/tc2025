public class MainThreadPi {
	private static int NUM_RECTS = 100_000_000;
	private static final int MAXTHREADS = Runtime.getRuntime().availableProcessors();
	
	public static void main(String args[]) {
		int block;
		long startTime, stopTime;
		double width, area, acum = 0;
		
		width = 1.0 / (double) NUM_RECTS;
		block = NUM_RECTS / MAXTHREADS;
		ThreadPi threads[] = new ThreadPi[MAXTHREADS];
		
		acum = 0;
		for (int j = 0; j < Utils.N; j++) {
			for (int i = 0; i < threads.length; i++) {
				if (i != threads.length - 1) {
					threads[i] = new ThreadPi(width, (i * block), ((i + 1) * block));
				} else {
					threads[i] = new ThreadPi(width, (i * block), NUM_RECTS);
				}
			}
			
			startTime = System.currentTimeMillis();
			for (int i = 0; i < threads.length; i++) {
				threads[i].start();
			}
			
			for (int i = 0; i < threads.length; i++) {
				try {
					threads[i].join();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			stopTime = System.currentTimeMillis();
			acum +=  (stopTime - startTime);
		}
		
		double aux = 0;
		for (int i = 0; i < threads.length; i++) {
			aux += threads[i].getSum();
		}
		area = width * aux;
		System.out.printf("sum = %.15f\n", area);
		System.out.printf("avg time = %.5f\n", (acum / Utils.N));
	}
}
