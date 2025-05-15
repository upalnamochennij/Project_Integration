import 'package:flutter/material.dart';
import 'package:table_calendar/table_calendar.dart';

class SymptomTracker extends StatefulWidget {
  const SymptomTracker({super.key});

  @override
  State<SymptomTracker> createState() => _SymptomTrackerState();
}

class _SymptomTrackerState extends State<SymptomTracker> {
  final List<String> _symptoms = [
    'Headache',
    'Cough',
    'Fatigue',
    'Fever',
    'Nausea',
  ];

  // Map of date → list of symptoms logged on that date
  final Map<DateTime, List<String>> _logs = {};

  // Today’s/checklist selections for the currently selected day
  late Map<String, bool> _selected;

  // Calendar state
  DateTime _focusedDay = DateTime.now();
  DateTime _selectedDay = DateTime.now();

  @override
  void initState() {
    super.initState();
    _initDaySelection(_selectedDay);
  }

  // Initialize or reload the checklist for a given day
  void _initDaySelection(DateTime day) {
    final key = DateTime(day.year, day.month, day.day);
    final existing = _logs[key] ?? [];
    _selected = {for (var s in _symptoms) s: existing.contains(s)};
  }

  void _saveSelection() {
    final key = DateTime(
      _selectedDay.year,
      _selectedDay.month,
      _selectedDay.day,
    );
    final picked =
        _selected.entries.where((e) => e.value).map((e) => e.key).toList();

    setState(() {
      if (picked.isEmpty) {
        _logs.remove(key);
      } else {
        _logs[key] = picked;
      }
      // leave _selected as-is so the checkboxes reflect the saved state
    });

    final msg =
        picked.isEmpty
            ? 'Cleared symptoms for ${_format(key)}'
            : 'Logged ${picked.length} symptom(s) for ${_format(key)}';
    ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text(msg)));
  }

  String _format(DateTime d) =>
      '${d.year}-${d.month.toString().padLeft(2, '0')}-${d.day.toString().padLeft(2, '0')}';

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Symptom Tracker',
        style: TextStyle(fontWeight: FontWeight.bold),),
        backgroundColor: const Color(0xFFFFDADC),
        foregroundColor: Colors.black87,
      ),
      body: Column(
        children: [
          // ───── CALENDAR ─────
          TableCalendar(
            firstDay: DateTime.utc(2020, 1, 1),
            lastDay: DateTime.utc(2030, 12, 31),
            focusedDay: _focusedDay,
            selectedDayPredicate: (d) => isSameDay(d, _selectedDay),
            onDaySelected: (sel, foc) {
              setState(() {
                _selectedDay = sel;
                _focusedDay = foc;
                _initDaySelection(sel);
              });
            },
            calendarBuilders: CalendarBuilders(
              defaultBuilder: (ctx, day, _) {
                final key = DateTime(day.year, day.month, day.day);
                if (_logs.containsKey(key)) {
                  return Container(
                    margin: const EdgeInsets.all(6),
                    decoration: BoxDecoration(
                      color: Colors.redAccent.withOpacity(0.3),
                      shape: BoxShape.circle,
                    ),
                    alignment: Alignment.center,
                    child: Text('${day.day}'),
                  );
                }
                return null;
              },
            ),
          ),

          const SizedBox(height: 8),
          Text(
            'Selected: ${_format(_selectedDay)}',
            style: const TextStyle(fontWeight: FontWeight.bold),
          ),
          const Divider(),

          // ───── CHECKBOXES FOR SELECTED DAY ─────
          Expanded(
            child: ListView(
              padding: const EdgeInsets.symmetric(horizontal: 16),
              children: [
                const SizedBox(height: 8),
                const Text('Symptoms:', style: TextStyle(fontSize: 16)),
                ..._symptoms.map(
                  (sym) => CheckboxListTile(
                    title: Text(sym),
                    value: _selected[sym],
                    onChanged: (v) {
                      setState(() => _selected[sym] = v ?? false);
                    },
                  ),
                ),
              ],
            ),
          ),

          // ───── SAVE BUTTON ─────
          Padding(
            padding: const EdgeInsets.all(16),
            child: ElevatedButton(
              style: ElevatedButton.styleFrom(
                backgroundColor: const Color(0xFFFFDADC),
                minimumSize: const Size.fromHeight(48),
              ),
              onPressed: _saveSelection,
              child: const Text(
                'Save for Selected Day',
                style: TextStyle(fontSize: 20, color: Colors.red),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
