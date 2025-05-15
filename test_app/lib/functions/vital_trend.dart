import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:intl/intl.dart';

class VitalTrend extends StatefulWidget {
  const VitalTrend({super.key});

  @override
  State<VitalTrend> createState() => _VitalTrendState();
}

class _VitalTrendState extends State<VitalTrend> {
  int selectedIndex = 0;
  DateTimeRange? selectedDateRange;

  final List<String> titles = [
    'Heart Rate',
    'Foot Steps',
    'Body Temperature',
    'Blood Oxygen',
  ];

  final List<IconData> icons = [
    Icons.favorite,
    Icons.directions_walk, // or use FontAwesomeIcons.shoePrints with font_awesome_flutter
    Icons.thermostat,
    Icons.bloodtype,
  ];

  final List<List<double>> allData = [
    [72, 75, 78, 76, 74, 73, 77],
    [5200, 6200, 5800, 6100, 5900, 6000, 6300], // foot steps
    [36.6, 36.7, 36.8, 36.5, 36.6, 36.7, 36.8],
    [98, 97, 99, 98, 96, 97, 98],
  ];

  final List<Color> colors = [
    Colors.redAccent,
    Colors.blueAccent, // footsteps color
    Colors.orangeAccent,
    Colors.green,
  ];


  final List<String> days = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];

  @override
  Widget build(BuildContext context) {
    final data = allData[selectedIndex];
    final minY = (data.reduce((a, b) => a < b ? a : b) - (selectedIndex == 1 ? 200 : 5)).toDouble();
    final maxY = (data.reduce((a, b) => a > b ? a : b) + (selectedIndex == 1 ? 200 : 5)).toDouble();

    return Scaffold(
      backgroundColor: const Color(0xFFFFE5E7),
      appBar: AppBar(
        backgroundColor: const Color(0xFFFFE5E7),
        elevation: 0,
        leading: IconButton(
          icon: const Icon(Icons.arrow_back, color: Colors.black87),
          onPressed: () => Navigator.of(context).pop(),
        ),
        title: const Text(
          'Vital Trends',
          style: TextStyle(color: Colors.black87),
        ),
        centerTitle: true,

      ),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            // Date Range Picker Button
            Align(
              alignment: Alignment.centerRight,
              child: ElevatedButton.icon(
                icon: const Icon(Icons.date_range),
                label: Text(
                  selectedDateRange == null
                      ? "Select Date Range"
                      : "${DateFormat('MMMd').format(selectedDateRange!.start)} - ${DateFormat('MMMd').format(selectedDateRange!.end)}",
                ),
                onPressed: () async {
                  final now = DateTime.now();
                  final picked = await showDateRangePicker(
                    context: context,
                    firstDate: DateTime(now.year - 1),
                    lastDate: DateTime(now.year + 1),
                    initialDateRange: selectedDateRange,
                  );
                  if (picked != null) {
                    setState(() {
                      selectedDateRange = picked;
                    });
                  }
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: Colors.white,
                  foregroundColor: Colors.black87,
                ),
              ),
            ),
            const SizedBox(height: 16),
            Container(
              width: double.infinity,
              padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 32),
              decoration: BoxDecoration(
                color: const Color(0xFFFFF4F2),
                borderRadius: BorderRadius.circular(40),
              ),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  const Text(
                    'Health History',
                    style: TextStyle(
                      fontSize: 24,
                      fontWeight: FontWeight.bold,
                      color: Colors.black87,
                    ),
                  ),
                  const SizedBox(height: 24),
                  SizedBox(
                    height: 240,
                    child: LineChart(
                      LineChartData(
                        minY: minY,
                        maxY: maxY,
                        lineTouchData: LineTouchData(
                          touchTooltipData: LineTouchTooltipData(
                            tooltipBgColor: Colors.white.withOpacity(0.9),
                            tooltipRoundedRadius: 10,
                            fitInsideHorizontally: true,
                            getTooltipItems: (touchedSpots) {
                              return touchedSpots.map((spot) {
                                return LineTooltipItem(
                                  "${spot.y.toStringAsFixed(1)}",
                                  const TextStyle(
                                    fontSize: 14,
                                    fontWeight: FontWeight.bold,
                                    color: Colors.black87,
                                  ),
                                );
                              }).toList();
                            },
                          ),
                          handleBuiltInTouches: true,
                          getTouchedSpotIndicator:
                              (barData, spotIndexes) => spotIndexes
                              .map((index) => TouchedSpotIndicatorData(
                            FlLine(
                                color:
                                colors[selectedIndex].withOpacity(0.3),
                                strokeWidth: 1),
                            FlDotData(show: true),
                          ))
                              .toList(),
                        ),
                        titlesData: FlTitlesData(
                          bottomTitles: AxisTitles(
                            sideTitles: SideTitles(
                              showTitles: true,
                              interval: 1, // ensure one label per index
                              getTitlesWidget: (value, meta) {
                                if (value.toInt() >= 0 && value.toInt() < days.length) {
                                  return SideTitleWidget(
                                    axisSide: meta.axisSide,
                                    child: Text(
                                      days[value.toInt()],
                                      style: const TextStyle(fontSize: 12),
                                    ),
                                  );
                                }
                                return const SizedBox.shrink();
                              },
                            ),
                          ),

                          leftTitles: AxisTitles(
                            sideTitles: SideTitles(
                              showTitles: true,
                              interval: selectedIndex == 1 ? 200 : 5, // custom interval for foot steps
                              reservedSize: selectedIndex == 1 ? 55 : 40, // more space for 4-digit numbers
                              getTitlesWidget: (value, _) => Text(
                                value.toStringAsFixed(0), // full value, like 5000
                                style: const TextStyle(fontSize: 12),
                              ),
                            ),
                          ),

                          topTitles:
                          AxisTitles(sideTitles: SideTitles(showTitles: false)),
                          rightTitles:
                          AxisTitles(sideTitles: SideTitles(showTitles: false)),
                        ),
                        gridData: FlGridData(show: true),
                        borderData: FlBorderData(show: true),
                        lineBarsData: [
                          LineChartBarData(
                            spots: data
                                .asMap()
                                .entries
                                .map((e) => FlSpot(e.key.toDouble(), e.value))
                                .toList(),
                            isCurved: true,
                            color: colors[selectedIndex],
                            barWidth: 3,
                            isStrokeCapRound: true,
                            dotData: FlDotData(show: false),
                            belowBarData: BarAreaData(
                              show: true,
                              color: colors[selectedIndex].withOpacity(0.2),
                            ),
                          ),
                        ],
                      ),
                    ),
                  ),
                  const SizedBox(height: 30),
                  Wrap(
                    spacing: 12,
                    runSpacing: 12,
                    alignment: WrapAlignment.center,
                    children: List.generate(titles.length, (index) {
                      return ElevatedButton.icon(
                        onPressed: () => setState(() => selectedIndex = index),
                        icon: Icon(
                          icons[index],
                          size: 18,
                          color: selectedIndex == index
                              ? Colors.white
                              : Colors.black54,
                        ),
                        label: Text(
                          titles[index],
                          style: const TextStyle(fontWeight: FontWeight.w600),
                        ),
                        style: ElevatedButton.styleFrom(
                          backgroundColor: selectedIndex == index
                              ? colors[index].withOpacity(0.9)
                              : Colors.grey[200],
                          foregroundColor: selectedIndex == index
                              ? Colors.white
                              : Colors.black87,
                          elevation: selectedIndex == index ? 6 : 2,
                          padding: const EdgeInsets.symmetric(
                              horizontal: 20, vertical: 12),
                          shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(20),
                          ),
                        ),
                      );
                    }),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
