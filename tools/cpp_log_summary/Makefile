PY = .venv/bin/python

quick-demo:
	@echo "🔁 Running weekly energy simulation..."
	@$(PY) simulator/simulate_week.py || echo "Simulation failed (is venv active and matplotlib installed?)"

log-summary:
	@cd tools/cpp_log_summary && g++ -O2 log_summary.cpp -o log_summary
	@cd tools/cpp_log_summary && ./log_summary ../../data/prototype_log.csv
