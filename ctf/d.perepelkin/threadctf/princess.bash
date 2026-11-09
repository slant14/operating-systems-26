killed=""
count=0
while [ $count -lt 50 ]; do
  for ppid in $(ps -eo stat,ppid,comm | awk '$1 ~ /^Z/ && $2 > 1 {print $2}'); do
    case " $killed " in
    *" $ppid "*) ;;
    *)
      if kill -9 "$ppid" 2>/dev/null; then
        killed="$killed $ppid"
        count=$((count + 1))
        echo "[$count/50] Torn down cabin PID $ppid"
      fi
      ;;
    esac
  done
  sleep 0.1
done
