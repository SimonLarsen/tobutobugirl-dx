package dk.hisimon.mmlgb;

import java.io.*;
import java.nio.file.*;
import java.util.List;

public class MMLGB {
	public static void main (String[] args) throws Exception {
		if(args.length < 2 || args.length > 3) {
			System.err.println("error: Wrong number of arguments.");
			System.err.println("Usage: java -jar MMLGB.jar MMLFILE OUTFILE [BANK]");
			System.exit(1);
		}

		Path inpath = FileSystems.getDefault().getPath(args[0]);
		String input = new String(Files.readAllBytes(inpath));

		Path outpath = FileSystems.getDefault().getPath(args[1]);

		List<Lexer.Token> tokens = Lexer.lex(input);
		Parser parser = new Parser(tokens);
		Song song = parser.parse();

		song.setFileName(outpath.getFileName().toString());
		PrintWriter pw = new PrintWriter(outpath.toFile());

		if(args[1].endsWith(".asm") || args[1].endsWith(".ASM")
		|| args[1].endsWith(".s") || args[1].endsWith(".S")) {
			if(args.length == 3) {
				int bank = Integer.parseInt(args[2]);
				pw.print(String.format(".area _CODE_%d\n", bank));
			}
			pw.print(song.emitASM());
		}
		if(args[1].endsWith(".h") || args[1].endsWith(".H")) {
			pw.print(song.emitC());
		}

		pw.flush();
		pw.close();
	}
}
