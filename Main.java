package studio2;

import java.io.CharArrayReader;

public class Main {

	/**
	 * Exercise the parser and scanner on some examples.
	 * @param args
	 */
	public static void main(String[] args) throws Throwable {
		//
		// Add more examples to this array
		//   Each will be tried through the parser
		//
		String[] examples = new String[] {
		/* 1 */ 		"f b   i a   a = 5   b = a + 3.2   p b",
		/* 2 */ 		"f a",
		/* 3 */ 		"a a",
		/* 4 */ 		"f f",
		/* 5 */ 		"a = 5  f b",
		/* 6 */ 		"% %"
		};

		for (String example : examples) {
			try {
				System.out.println("Parsing: " + example);
				CharArrayReader reader = new CharArrayReader(example.toCharArray());
				CharStream s = new CharStream(reader);

				Parser p = new Parser(s);
				p.Prog();
				System.out.println("   Parse successful");
			}
			catch (Throwable t) {
				System.out.println("   Parse ended with error: " + t);
				System.out.println("Stack trace: ");
				t.printStackTrace(System.out);
			}
		}

	}

}
