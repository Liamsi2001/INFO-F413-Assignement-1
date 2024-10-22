import matplotlib.pyplot as plt
import pandas as pd

# Read CSV files into pandas DataFrames for faster processing
quick_df = pd.read_csv('quick_selection_data.csv')
lazy_df = pd.read_csv('lazy_selection_data.csv')

# Filter out 'Average' runs upfront for both QuickSelect and LazySelect
quick_avg_df = quick_df[quick_df['Run'] == 'Average']
lazy_avg_df = lazy_df[lazy_df['Run'] == 'Average']

# Group by array size and k type for QuickSelect
quick_grouped = quick_avg_df.groupby(['ArraySize', 'Kth']).agg({
    'EmpiricalComparisons': 'mean',
    'RunningTime(us)': 'mean',
    'ExpectedComparisons': 'first'
}).reset_index()

# Group by array size and k type for LazySelect
lazy_grouped = lazy_avg_df.groupby(['ArraySize', 'Kth']).agg({
    'EmpiricalComparisons': 'mean',
    'RunningTime(us)': 'mean',
    'ExpectedComparisons': 'first'
}).reset_index()

# Plotting individual instances along with averages
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 8))

# Set transparency level for individual instances
alpha_value = 0.5  # For lighter plotting of individual runs

markers = ['o', 'x', '+']  # Different markers for 'First', '25%', 'Median'

# Plot QuickSelect individual instances first
for k_type, color, marker in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal'], markers):
    quick_data = quick_df[quick_df['Kth'] == k_type]
    ax1.scatter(quick_data['ArraySize'], quick_data['EmpiricalComparisons'], 
                label=f'{k_type} Instances', color=color, alpha=alpha_value, marker=marker)

# Then plot QuickSelect averages
for k_type, color, marker in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal'], markers):
    quick_avg = quick_grouped[quick_grouped['Kth'] == k_type]
    ax1.plot(quick_avg['ArraySize'], quick_avg['EmpiricalComparisons'], 
             label=f'{k_type} Average', color=color, marker=marker, linewidth=2)

# Add expected comparisons
ax1.plot(quick_grouped['ArraySize'].unique(), quick_grouped['ExpectedComparisons'].unique(), 
         label='Expected Comparisons (3.386 * n)', color='darkgoldenrod', linestyle='--', linewidth=2)

# Apply logarithmic scale
ax1.set_xscale('log')  # Logarithmic scale for array sizes

# Add labels and title
ax1.set_xlabel('Array Size (log scale)')
ax1.set_ylabel('Number of Comparisons')
ax1.set_title('QuickSelect: Instances and Averages for Different k Values')
ax1.legend(loc='upper left')
ax1.grid(True)

# Plot LazySelect individual instances first
for k_type, color, marker in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal'], markers):
    lazy_data = lazy_df[lazy_df['Kth'] == k_type]
    ax2.scatter(lazy_data['ArraySize'], lazy_data['EmpiricalComparisons'], 
                label=f'{k_type} Instances', color=color, alpha=alpha_value, marker=marker)

# Then plot LazySelect averages
for k_type, color in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal']):
    lazy_avg = lazy_grouped[lazy_grouped['Kth'] == k_type]
    ax2.plot(lazy_avg['ArraySize'], lazy_avg['EmpiricalComparisons'], 
             label=f'{k_type} Average', color=color, marker='o', linewidth=2)

# Add expected comparisons
ax2.plot(lazy_grouped['ArraySize'].unique(), lazy_grouped['ExpectedComparisons'].unique(), 
         label='Expected Comparisons (2 * n)', color='darkgoldenrod', linestyle='--', linewidth=2)

# Apply logarithmic scale
ax2.set_xscale('log')  # Logarithmic scale for array sizes

# Add labels and title
ax2.set_xlabel('Array Size (log scale)')
ax2.set_ylabel('Number of Comparisons')
ax2.set_title('LazySelect: Instances and Averages for Different k Values')
ax2.legend(loc='upper left')
ax2.grid(True)

# Save figure
plt.tight_layout()
plt.savefig('comparison_plots.png', dpi=300)

# Plot running times similarly
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 8))

# Plot QuickSelect running times for individual instances first
for k_type, color, marker in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal'], markers):
    quick_data = quick_df[quick_df['Kth'] == k_type]
    ax1.scatter(quick_data['ArraySize'], quick_data['RunningTime(us)'], 
                label=f'{k_type} Instances', color=color, alpha=alpha_value, marker=marker)

# Then plot QuickSelect averages
for k_type, color in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal']):
    quick_avg = quick_grouped[quick_grouped['Kth'] == k_type]
    ax1.plot(quick_avg['ArraySize'], quick_avg['RunningTime(us)'], 
             label=f'{k_type} Average', color=color, marker='o', linewidth=2)

# Apply logarithmic scale
ax1.set_xscale('log')  # Logarithmic scale for array sizes

# Add labels and title for QuickSelect
ax1.set_xlabel('Array Size (log scale) ')
ax1.set_ylabel('Running Time (us) ')
ax1.set_title('QuickSelect: Instances and Averages Running Time for Different k Values')
ax1.legend(loc='upper left')
ax1.grid(True)

# Plot LazySelect running times for individual instances first
for k_type, color, marker in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal'], markers):
    lazy_data = lazy_df[lazy_df['Kth'] == k_type]
    ax2.scatter(lazy_data['ArraySize'], lazy_data['RunningTime(us)'], 
                label=f'{k_type} Instances', color=color, alpha=alpha_value, marker=marker)

# Then plot LazySelect averages
for k_type, color in zip(['First', '25%', 'Median'], ['indianred', 'rebeccapurple', 'teal']):
    lazy_avg = lazy_grouped[lazy_grouped['Kth'] == k_type]
    ax2.plot(lazy_avg['ArraySize'], lazy_avg['RunningTime(us)'], 
             label=f'{k_type} Average', color=color, marker='o', linewidth=2)

# Apply logarithmic scale
ax2.set_xscale('log')  # Logarithmic scale for array sizes

# Add labels and title for LazySelect
ax2.set_xlabel('Array Size (log scale) ') 
ax2.set_ylabel('Running Time (us)')
ax2.set_title('LazySelect: Instances and Averages Running Time for Different k Values')
ax2.legend(loc='upper left')
ax2.grid(True)

# Save figure
plt.tight_layout()
plt.savefig('time_comparison_plots.png', dpi=300)


