import java.util.Random;

public class Utils {
	private static final int DISPLAY = 100;
	private static final int MAX_VALUE = 10_000;
	private static final Random r = new Random();
	public static final int N = 10;
	
	public static void randomArray(int array[]) {
		for (int i = 0; i < array.length; i++) {
			array[i] = r.nextInt(MAX_VALUE) + 1;
		}
	}
	
	public static void fillArray(int array[]) {
		for (int i = 0; i < array.length; i++) {
			array[i] = (i % MAX_VALUE) + 1;
		}
	}
	
	public static void displayArray(String text, int array[]) {
		System.out.printf("%s = [%4d", text, array[0]);
		for (int i = 1; i < DISPLAY; i++) {
			System.out.printf(",%4d", array[i]);
		}
		System.out.printf(", ..., ]\n");
	}
}
