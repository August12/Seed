import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class SmallestTriangle {

	public static class Point {
		public final double x;
		public final double y;
		public final int index;

		public Point(int index, double x, double y) {
			this.index = index;
			this.x = x;
			this.y = y;
		}

	}

	public static class Pair {
		public Point point1 = null;
		public Point point2 = null;
		public Point point3 = null;
		public double distance = 0.0;
		public double distMax = 0.0;

		public Pair() {
			// empty
		}

		public Pair(Point point1, Point point2, Point point3) {
			this.point1 = point1;
			this.point2 = point2;
			this.point3 = point3;
			find_distance();
		}

		public void update(Point point1, Point point2, Point point3, double distance, double distMax) {
			this.point1 = point1;
			this.point2 = point2;
			this.point3 = point3;
			this.distance = distance;
			this.distMax = distMax;
		}

		public void find_distance() {
			double distance12 = cal_distance(point1, point2);
			double distance13 = cal_distance(point1, point3);
			double distance23 = cal_distance(point2, point3);
			this.distance = distance12 + distance13 + distance23;
			this.distMax = (distance12 > distance13) ? (distance12 > distance23? distance12:distance23) : (distance13 > distance23? distance13:distance23);
		}

		public String toString() {
			return point1.index + "\n" + point2.index + "\n" + point3.index;
		}

	}

	public static double cal_distance(Point p1, Point p2) {
		double xdis = p2.x - p1.x;
		double ydis = p2.y - p1.y;
		return Math.hypot(xdis, ydis);
	}

	public static Pair BruteForce(List<Point> points) {
		int point_num = points.size();
		if (point_num < 3)
			return null;
		Pair pair = new Pair(points.get(0), points.get(1), points.get(2));
		double bound = pair.distance/2;
		for (int i = 0; i < point_num - 2; i++) {
			Point p1 = points.get(i);
			for (int j = i + 1; j < point_num - 1; j++) {
				Point p2 = points.get(j);
				if(Math.abs(p2.x - p1.x) >= bound)
	                  break;
				double distance12 = cal_distance(p1, p2);
				if(distance12 >= bound)
					continue;
				
				for (int k = j + 1; k < point_num; k++) {
					Point p3 = points.get(k);
					if(Math.abs(p3.x - p1.x) >= bound || Math.abs(p3.x - p2.x) >= bound)
	                     break;
					double distance13 = cal_distance(p1, p3);
					double distance23 = cal_distance(p2, p3);
					
					if(distance13 >= bound || distance23 >= bound)
						continue;
					
					double distance = distance12 + distance13 + distance23;
					
					if (distance < pair.distance) {
						double distMax = (distance12 > distance13) ? (distance12 > distance23? distance12:distance23) : (distance13 > distance23? distance13:distance23);
						pair.update(p1, p2, p3, distance, distMax);
						bound = distance / 2;
					}
				}
				

				
			}
		}
		return pair;
	}

	public static void sortX(List<Point> points) {
		Collections.sort(points, new Comparator<Point>() {
			public int compare(Point p1, Point p2) {
				if (p1.x < p2.x)
					return -1;
				else if (p1.x > p2.x)
					return 1;
				return 0;
			}
		});
	}

	public static Pair Divide_Conquer_Sort(List<Point> points) {
		sortX(points);
		return Divide_Conquer(points);
	}

	private static Pair Divide_Conquer(List<Point> xP) {
		int N = xP.size();

		if (N <= 4)
			return BruteForce(xP);
		int divide_index = N / 2;

		List<Point> xL = xP.subList(0, divide_index);
		List<Point> xR = xP.subList(divide_index, N);
		double xm = xR.get(0).x;

		Pair pairL = Divide_Conquer(xL);
		Pair pairR = Divide_Conquer(xR);

		Pair closestPair = new Pair();
		if (pairR == null)
			closestPair = pairL;
		else if(pairL == null)
			closestPair = pairR;
		else if (pairR.distance < pairL.distance)
			closestPair = pairR;
		else if (pairR.distance > pairL.distance)
			closestPair = pairL;

		double dmin = closestPair.distMax;
		List<Point> xS = new ArrayList<Point>();
		for (Point point : xP) {
			if (Math.abs(xm - point.x) < dmin)
				xS.add(point);
		}
		
		Pair ans = BruteForce(xS);
		if(ans != null && ans.distance < closestPair.distance) {
			closestPair = ans;
		}

		return closestPair;
	}

	static void ReadTxt(String path, List<Point> points) throws IOException {
		try {
			BufferedReader in = new BufferedReader(new FileReader(path));
			String s;
			while ((s = in.readLine()) != null) {
				String[] line = s.split(",");
				if (line.length < 2)
					continue;
				int index = Integer.parseInt(line[0]);
				double x = Double.parseDouble(line[1]);
				double y = Double.parseDouble(line[2]);
				points.add(new Point(index, x, y));
			}
			in.close();
		} catch (IOException e) {
			System.err.println(e);
			System.exit(1);
		}
	}

	public static void main(String[] args) throws IOException {
		List<Point> point = new ArrayList<Point>();
		ReadTxt(args[0], point);
//		long t2 = System.currentTimeMillis();
		Pair Divide_Conquer = Divide_Conquer_Sort(point);
//		System.out.println("time (millisec) : " + (System.currentTimeMillis() - t2));
		System.out.println(Divide_Conquer);
	}
}