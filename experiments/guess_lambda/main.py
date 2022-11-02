from guess import guess_optimal_cfg, plot_values
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--config', help='path of 1 or more config files', nargs='+')
    args = parser.parse_args()
    guess_optimal_cfg(*args.config)

    print(args)

if __name__ == "__main__":
    main()
